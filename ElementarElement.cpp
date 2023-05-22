#include "ElementarElement.h"
#include "Quaternion.h"


ElementarElement::ElementarElement(float a, float b, float c,  float UpdateFrame)
{
	x = a;
	y = b;
	z = c;


	dt = 1.0 / UpdateFrame;

}


void ElementarElement::Starting_conditions_for_W()
{
	Wx = 0;
	Wy = 0;
	Wz = 0;
}

void ElementarElement::Starting_conditions_for_V(float v, float u, float y)
{
	Vx = v;
	Vy = u;
	Vz = y;
}

void ElementarElement::Starting_conditions_for_M(float m)
{
	M = m;
	R = R0 * pow(m / M0, 1.0 / 3);
}


void ElementarElement::Move()
{
	Vx += Wx * dt;
	Vy += Wy * dt;
	Vz = Wz * dt;

	x += Vx * dt;
	y += Vy * dt;
	z += Vz * dt;

}
void ElementarElement::GravitationalForce(ElementarElement* element)
{
	float r = pow(pow((x - element->x), 2) + pow((y - element->y), 2) + pow((z - element->z), 2), 3.0 / 2);

	Wx += G * element->M * (element->x - x) / r;
	Wy += G * element->M * (element->y - y) / r;
	Wz += G * element->M * (element->z - z) / r;

	element->Wx -= G * M * (element->x - x) / r;
	element->Wy -= G * M * (element->y - y) / r;
	element->Wz -= G * M * (element->z - z) / r;
}

void ElementarElement::Cout()
{
	std::cout << "x: " << x << " y: " << y << "z = " << z <<" Vx: " << Vx << " Vy : " << Vy << "Wx : " << Wx << "Wy : " << Wy << " M = " << M << " R = " << R << '\n';
}

bool ElementarElement::ClashReporter(ElementarElement& element)
{
	if (pow((x - element.x), 2) + pow((y - element.y), 2) + pow((z - element.z), 2) < pow((R + element.R), 2)) {
		return true;
	}
	return false;
}

void ElementarElement::InelasticClashForce(ElementarElement* element)
{
	float Sum_M = M + element->M;
	if (element->M < M) {
		Vx = (M * Vx + element->M * element->Vx) / Sum_M;
		Vy = (M * Vy + element->M * element->Vy) / Sum_M;
		Vz = (M * Vz + element->M * element->Vz) / Sum_M;

		R = pow(pow(R, 3) + pow(element->R, 3), 1.0 / 3);

		M = Sum_M;


	}
	else {
		element->Vx = (M * Vx + element->M * element->Vx) / Sum_M;
		element->Vy = (M * Vy + element->M * element->Vy) / Sum_M;
		element->Vz = (M * Vz + element->M * element->Vz) / Sum_M;

		element->R = pow(pow(R, 3) + pow(element->R, 3), 1.0 / 3);

		element->M = Sum_M;

	}
}

void ElementarElement::CoordFor3D(Quaternion camerapos, Quaternion camerarotation)
{

	Quaternion pos(0, x, y, z);

	double distanceFromCamera;
	Quaternion draw_pos;

	draw_pos = camerarotation * ((pos - camerapos) * camerarotation.inverse());

	shape.setFillColor(sf::Color(std::rand() % 255, std::rand() % 255, std::rand() % 255));
	distanceFromCamera = draw_pos.get_magnitude();

	render_radius = R/ distanceFromCamera * 1000;

	if (draw_pos.z < 0)
		shape.setRadius(0);
	else
		shape.setRadius(render_radius);

	shape.setPosition(draw_pos.getScreenPos() + sf::Vector2f(-render_radius, -render_radius));

}

void ElementarElement::draw(sf::RenderWindow& window)
{
	window.draw(shape);
}
