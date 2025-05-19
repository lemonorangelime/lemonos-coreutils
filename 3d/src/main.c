#include <string.h>
#include <graphics.h>
#include <stdlib.h>
#include <sched.h>
#include <3d.h>
#include <stdio.h>

// note: this code is abyismal, but the idea is just to test that 3d is working

int main(int argc, char * argv[]) {
	window_t * window = create_window(u"3d", u"3d", 200, 200);
	rect_2d_t * rect = &window->rect;
	camera_t camera = {(vect_3d_t) {0.0, 0.0, 0.0}, 200};

	tri_3d_t triangle = {(vect_3d_t) {0.0, 0.0, 20.0}, (vect_3d_t) {5.0, 0.0, 20.0}, (vect_3d_t) {0.0, 5.0, 20.0},  0xffff0000};
	tri_3d_t triangle2 = {(vect_3d_t) {0.0, 5.0, 20.0}, (vect_3d_t) {5.0, 0.0, 20.0}, (vect_3d_t) {5.0, 5.0, 20.0}, 0xff00ff00};

	tri_3d_t triangle3 = {(vect_3d_t) {0.0, 5.0, 20.0}, (vect_3d_t) {0.0, 0.0, 25.0}, (vect_3d_t) {0.0, 0.0, 20.0}, 0xff0000ff};
	tri_3d_t triangle4 = {(vect_3d_t) {0.0, 5.0, 25.0}, (vect_3d_t) {0.0, 0.0, 25.0}, (vect_3d_t) {0.0, 5.0, 20.0}, 0xffff00ff};

	tri_3d_t triangle5 = {(vect_3d_t) {5.0, 0.0, 20.0}, (vect_3d_t) {5.0, 0.0, 25.0}, (vect_3d_t) {5.0, 5.0, 20.0}, 0xffffff00};
	tri_3d_t triangle6 = {(vect_3d_t) {5.0, 5.0, 20.0}, (vect_3d_t) {5.0, 0.0, 25.0}, (vect_3d_t) {5.0, 5.0, 25.0}, 0xff00ffff};

	tri_3d_t triangle7 = {(vect_3d_t) {0.0, 0.0, 20.0}, (vect_3d_t) {0.0, 0.0, 25.0}, (vect_3d_t) {5.0, 0.0, 20.0}, 0xff800080};
	tri_3d_t triangle8 = {(vect_3d_t) {5.0, 0.0, 25.0}, (vect_3d_t) {5.0, 0.0, 20.0}, (vect_3d_t) {0.0, 0.0, 25.0}, 0xff208000};

	tri_3d_t triangle9 = {(vect_3d_t) {0.0, 5.0, 25.0}, (vect_3d_t) {5.0, 0.0, 25.0}, (vect_3d_t) {0.0, 0.0, 25.0},  0xff200080};
	tri_3d_t triangle10 = {(vect_3d_t) {5.0, 5.0, 25.0}, (vect_3d_t) {5.0, 0.0, 25.0}, (vect_3d_t) {0.0, 5.0, 25.0}, 0xff004020};

	tri_3d_t triangle11 = {(vect_3d_t) {5.0, 5.0, 20.0}, (vect_3d_t) {0.0, 5.0, 25.0}, (vect_3d_t) {0.0, 5.0, 20.0}, 0xff808020};
	tri_3d_t triangle12 = {(vect_3d_t) {0.0, 5.0, 25.0}, (vect_3d_t) {5.0, 5.0, 20.0}, (vect_3d_t) {5.0, 5.0, 25.0}, 0xff208080};

	mesh_t mesh = {(vect_3d_t) {2.5, 2.5, 22.5}, (tri_3d_t *[]) {&triangle, &triangle2, &triangle3, &triangle4, &triangle5, &triangle6, &triangle7, &triangle8, &triangle9, &triangle10, &triangle11, &triangle12}, 12};

	vect_3d_t thing2 = {-2.5, -2.5, 0.0};
	transform_mesh(&mesh, &thing2);

	rect_2d_t * rect2 = malloc(sizeof(rect_2d_t));
	rect2->fb = malloc((200 * 200) * 4);
	rect2->size.width = 200;
	rect2->size.height = 200;

	memset32(rect->fb, 0xff000000, 200 * 200);
	memset32(rect2->fb, 0xffff0000, 200 * 200);

	rot_mesh(&mesh, 0.0, 1, 0.0);

	float speed = 0.05;
	while (1) {
		//vect_3d_t thing = {0, 0, 0};
		rot_mesh(&mesh, 0.0, 0.05, 0.0);
		rot_mesh(&mesh, 0.05, 0.0, 0.0);
		//rot_mesh(&mesh, 0.5, 0.0, 0.0);

		memset32(rect2->fb, 0xff000000, 200 * 200);
		rasterise_mesh(&mesh, rect2, &camera);

		memcpy32(rect->fb, rect2->fb, 200 * 200);
	}
}
