#include "Header.h"
#include "Vector3.h"
#include "rgb.h"
#include "Object.h"
#include "Sphere.h"
#include "Ray.h"
#include "Camera.h"
#include "easyppm.h"
#include <time.h>
#include <future>
#include <chrono>
#include <ctime>

struct block {
	std::vector<std::vector<rgb>> pixelColors;
	int x, y;
};

void denoise(PPM* ppm, int width, int height) {
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			rgb color(0, 0, 0);
			if (i + 1 < width && i - 1 > 0 && j + 1 < height && j - 1 > 0) {
				auto sample1 = easyppm_get(ppm, i - 1, j);
				auto sample2 = easyppm_get(ppm, i, j + 1);
				auto sample3 = easyppm_get(ppm, i + 1, j);
				auto sample4 = easyppm_get(ppm, i, j - 1);
				auto sample5 = easyppm_get(ppm, i - 1, j - 1);
				auto sample6 = easyppm_get(ppm, i - 1, j + 1);
				auto sample7 = easyppm_get(ppm, i + 1, j - 1);
				auto sample8 = easyppm_get(ppm, i + 1, j + 1);
				color = color + rgb(sample1.r, sample1.g, sample1.b);
				color = color + rgb(sample2.r, sample2.g, sample2.b);
				color = color + rgb(sample3.r, sample3.g, sample3.b);
				color = color + rgb(sample4.r, sample4.g, sample4.b);
				color = color + rgb(sample5.r, sample5.g, sample5.b);
				color = color + rgb(sample6.r, sample6.g, sample6.b);
				color = color + rgb(sample7.r, sample7.g, sample7.b);
				color = color + rgb(sample8.r, sample8.g, sample8.b);
			}
			color = color * (1.0 / 8);
			easyppm_set(ppm, i, j, easyppm_rgb(color.r, color.g, color.b));
		}
	}
}

rgb basicRayColor(const Ray& r, Object& scene, int bounceCount) {
	HitInfo hit;

	if (bounceCount > 10) {
		return rgb(0, 0, 0);
	}

	if (scene.intersect(r, 0.0001, INFINITY, hit)) {
		Ray scattered;
		rgb attenuation;
		if (hit.material->scatter(r, hit, attenuation, scattered)) {
			return attenuation * basicRayColor(scattered, scene, ++bounceCount);
		}
		return rgb(0, 0, 0);
	}

	// Gradient "sky" background
	Vector3 directionNormalized = r.direction.normalized();
	auto t = 0.5 * (directionNormalized.y + 1.0);
	return rgb(1.0, 1.0, 1.0)*(1.0 - t) + rgb(0.5, 0.7, 1.0)*t;
}

rgb colorSample(int sampleCount, int x, int y, Camera& cam, ObjectList& scene) {
	auto xd = x + randomDouble();
	auto yd = y + randomDouble();
	Ray temp1(Vector3(0, 0, 0), cam.pixelToWorld(xd, yd));

	return basicRayColor(temp1, scene, 0);
}

// Not really optimal waste of thread creation calls
block threadedBlockthreadedSamples(int sampleCount, int penis, int poop, int x, int y, Camera& cam, ObjectList& scene) {
	std::vector<std::future<rgb>> threadsSamples;
	block b;
	int yt = y;
	while (yt < penis) {
		int xt = x;
		std::vector<rgb> pixels;
		while (xt < poop) {
			rgb color(0, 0, 0);
			for (int s = 0; s < sampleCount; ++s) {
				threadsSamples.push_back(std::async(colorSample, sampleCount, xt, yt, std::ref<Camera>(cam), std::ref<ObjectList>(scene)));
			}

			for (int t = 0; t < threadsSamples.size(); ++t) {
				color = color + threadsSamples[t].get();
			}
			threadsSamples.clear();

			auto scale = 1.0 / sampleCount;
			color = color * scale;
			color.r = sqrtf(color.r);
			color.g = sqrtf(color.g);
			color.b = sqrtf(color.b);
			//easyppm_set(ppm, xt, yt, easyppm_rgb(255 * clamp(color.r, 1.0), 255 * clamp(color.g, 1.0), 255 * clamp(color.b, 1.0)));

			pixels.push_back(rgb(255 * clamp(color.r, 1.0), 255 * clamp(color.g, 1.0), 255 * clamp(color.b, 1.0)));

			++xt;
		}
		++yt;
		b.pixelColors.push_back(pixels);
	}
	return b;
}

block threadedBlock(int sampleCount, int blockX, int blockY, int xBlockScale, int yBlockScale, Camera& cam, ObjectList& scene) {
	// Initialize important stuff
	block b;
	b.x = blockX;
	b.y = blockY;
	int xInit = blockX * xBlockScale;
	int yInit = blockY * yBlockScale;
	int xBound = xInit + xBlockScale;
	int yBound = yInit + yBlockScale;


	rgb color(0, 0, 0);
	std::vector<rgb> pixels(xBlockScale);

	for (int yt = yInit; yt < yBound; ++yt) {
		for (int xt = xInit; xt < xBound; ++xt) {
			for (int s = 0; s < sampleCount; ++s) {
				auto xd = xt + randomDouble();
				auto yd = yt + randomDouble();
				Ray ray(Vector3(0, 0, 0), cam.pixelToWorld(xd, yd)); // Random ray per sample
				//Ray ray = cam.getRay(xd, yd);
				color = color + basicRayColor(ray, scene, 0);
			}
			auto scale = 1.0 / sampleCount;
			color = color * scale;
			color.r = sqrtf(color.r); // Correct gamma to 2.0
			color.g = sqrtf(color.g);
			color.b = sqrtf(color.b);

			auto index = xt - xInit;

			pixels[index] = (rgb(255 * clamp(color.r, 1.0), 255 * clamp(color.g, 1.0), 255 * clamp(color.b, 1.0)));
			color.zero();
		}
		b.pixelColors.push_back(pixels);
	}

	return b;
}

int main() {

	// Initialize important stuff
	const auto aspectRatio = 16.0 / 9.0;
	const int width = 1920;
	const int height = width / aspectRatio;
	int sampleCount = 100;

	int maxThreads = std::thread::hardware_concurrency();
	std::cout << "Threads: " << maxThreads << std::endl;
	std::vector<std::future<block>> threads;

	// Width height fov
	Camera cam(Vector3(-2, 2, 1), Vector3(0, 0, -1), Vector3(0, 0, -1), width, height, 70);

	/** ! TO DO ! Custom ppm creation easy ppm is annoying ! TO DO ! **/
	PPM ppm = easyppm_create(width, height, IMAGETYPE_PPM);

	ObjectList scene;

	/* ! TO DO !  Make a scene creation thing so these constructors don't litter main  ! TO DO ! */
	// Creating the scene
	scene.add(std::make_shared<Sphere>(Vector3(0, 0, -1), 0.5, std::make_shared<Lambertian>(rgb(0.7, 0.3, 0.3))));
	scene.add(std::make_shared<Sphere>(Vector3(-1, 0, -1), 0.5, std::make_shared<Dielectric>(1.5)));
	scene.add(std::make_shared<Sphere>(Vector3(1, 0, -1), 0.5, std::make_shared<Metal>(rgb(0.8, 0.6, 0.2), 0.6)));
	scene.add(std::make_shared<Sphere>(Vector3(1.5, 0, -1), 0.6, std::make_shared<Metal>(rgb(0.3, 0.9, 0.4), 0.1)));
	scene.add(std::make_shared<Sphere>(Vector3(0, -100.5, -1), 100, std::make_shared<Lambertian>(rgb(0.8, 0.8, 0))));

	// Chunk the image into minimum number of blocks
	int xBlockScale = 1;
	int yBlockScale = 1;
	for (unsigned int i = maxThreads; i > 0; --i) {
		/** ! TO DO ! This logic does not scale for strange height/width proportions ! TO DO ! **/
		if (width % i == 0 && height % i == 0) {
			xBlockScale = width / i;
			yBlockScale = height / i;
			break;
		}
	}
	int widthBlocks = width / xBlockScale;
	int heightBlocks = height / yBlockScale;
	std::vector<block> blockVec(widthBlocks * heightBlocks);
	int blocksLeft = blockVec.size();

	std::cout << "Blocks total: " << blocksLeft << std::endl;

	// This part makes bechmarking and manual profiling easier
	using namespace std::chrono;
	time_point<system_clock> start, end;
	start = system_clock::now();
	time_t start_time = system_clock::to_time_t(start);
	std::cout << "Started render at " << std::ctime(&start_time) << std::endl;

	// For percentage output
	int blocksDone = 0;
	int totalBlocks = widthBlocks * heightBlocks;
	float percentage = 0, percentageLast = 0;

	// Render loop
	for (int j = 0; j < heightBlocks; ++j) {
		int i = 0;
		while (i < widthBlocks) {
			/* ! TO DO ! Better thread batching algorithm would be nice. This seems too simple to be efficient ! TO DO !*/
			if (threads.size() < maxThreads) {
				threads.push_back(std::async(std::launch::async, threadedBlock, sampleCount, i, j, xBlockScale, yBlockScale, std::ref<Camera>(cam), std::ref<ObjectList>(scene)));
				++i;
				++blocksDone;
			}
			else {
				for (int t = 0; t < threads.size(); ++t) {
					if (threads[t].wait_for(std::chrono::seconds(-1)) == std::future_status::ready) {
						block temp = threads[t].get();
						blockVec[temp.x + temp.y * widthBlocks] = temp;
						threads.erase(threads.begin() + t);
						std::cout << "Blocks remaining: " << --blocksLeft << std::endl;
					}
				}
			}
		}
	}
	// Reclaim last batch of threads
	for (int i = 0; i < threads.size(); ++i) {
		block temp = threads[i].get();
		auto ind = temp.x + (temp.y * widthBlocks);
		blockVec[ind] = temp;
	}
	threads.clear();

	// End timing and output computation time
	end = system_clock::now();
	duration<double> elapsed_seconds = end - start;
	time_t end_time = system_clock::to_time_t(end);
	std::cout << "finished computation at " << std::ctime(&end_time)
		<< "elapsed time: " << elapsed_seconds.count() << "s\n";

	/* ! TO DO !  Make custom ppm writing function easyppm is heavy and annoying to use with rgb objects  ! TO DO ! */
	for (int blockY = 0; blockY < heightBlocks; ++blockY) {
		for (int blockX = 0; blockX < widthBlocks; ++blockX) {
			auto blockIndex = blockX + (blockY * widthBlocks);
			for (int y = 0; y < blockVec[blockIndex].pixelColors.size(); ++y) {
				for (int x = 0; x < blockVec[blockIndex].pixelColors[y].size(); ++x) {
					auto xPixel = x + (blockX * xBlockScale);
					auto yPixel = y + (blockY * yBlockScale);
					easyppm_set(&ppm, x + (blockX * xBlockScale), y + (blockY * yBlockScale), easyppm_rgb(blockVec[blockIndex].pixelColors[y][x].r, blockVec[blockIndex].pixelColors[y][x].g, blockVec[blockIndex].pixelColors[y][x].b));
				}
			}
		}
	}
	easyppm_write(&ppm, "image.ppm");
	easyppm_destroy(&ppm);

	system("pause");
}