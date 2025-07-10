#include "tgaimage.h"
#include <iostream>
#include "geometry.h"
#include "model.h"
#include <glm/glm.hpp>
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const int width = 800;
const int height = 800;


void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
		error2 += derror2;
		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

glm::vec3 barycentric(glm::vec2* pts, glm::vec2 p);

void triangle(glm::vec2* pts, TGAImage& image, TGAColor color);

int main()
{
	TGAImage image(width, height, TGAImage::RGBA);
	/*line5(13, 20, 80, 40, image, red);
	line5(20, 13, 40, 80, image, red);
	line5(80, 40, 13, 20, image, red);*/
	Model model("../obj/african_head/african_head.obj");
	//定义光线的方向
	glm::vec3 light_dir(0, 0, -1);

	//加载对应的模型顶点
	for (int i = 0; i < model.nfaces(); i++) {
		vec3 v0 = model.vert(i, 0);
		vec3 v1 = model.vert(i, 1);
		vec3 v2 = model.vert(i, 2);
		int x0 = (v0.x + 1.) * width / 2.;
		int y0 = (v0.y + 1.) * height / 2.;
		int x1 = (v1.x + 1.) * width / 2.;
		int y1 = (v1.y + 1.) * height / 2.;
		int x2 = (v2.x + 1.) * width / 2.;
		int y2 = (v2.y + 1.) * height / 2.;
		glm::vec2 pts[3] = { glm::vec2(x0, y0), glm::vec2(x1, y1), glm::vec2(x2, y2) };
		// 计算法线向量
		glm::vec3 normal = glm::normalize(glm::cross(
			glm::vec3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z),
			glm::vec3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z)
		));
		// 计算光照强度
		float intensity = glm::dot(normal, light_dir);


		//绘制对应的三角形
		triangle(pts, image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));


	}
	//测试对应的三角形渲染
	

	//image.flip_vertically();
	image.write_tga_file("output.tga");
	return 0;
}
glm::vec3 barycentric(glm::vec2* pts, glm::vec2 p)
{
	glm::vec3 u = glm::cross(glm::vec3(pts[2][0] - pts[0][0], pts[1][0] - pts[0][0], pts[0][0] - p[0]),
		glm::vec3(pts[2][1] - pts[0][1], pts[1][1] - pts[0][1], pts[0][1] - p[1]));

	if (std::abs(u.z) < 1) 		return glm::vec3(-1, -1, -1); // 0 area triangle

	return glm::vec3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}
void triangle(glm::vec2* pts, TGAImage& image, TGAColor color)
{
	// 计算包围盒
	int minX = std::min({ pts[0].x, pts[1].x, pts[2].x });
	int minY = std::min({ pts[0].y, pts[1].y, pts[2].y });
	int maxX = std::max({ pts[0].x, pts[1].x, pts[2].x });
	int maxY = std::max({ pts[0].y, pts[1].y, pts[2].y });
	// 遍历包围盒内的每个像素点
	for (int x = minX; x <= maxX; x++) {
		for (int y = minY; y <= maxY; y++) {
			glm::vec2 p(x, y);
			glm::vec3 bc = barycentric(pts, p);
			if (bc.x >= 0 && bc.y >= 0 && bc.z >= 0) {
				image.set(x, y, color);
			}
		}
	}

}