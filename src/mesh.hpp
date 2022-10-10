#include <vector>
#include <cmath>
#include "parser.hpp"

class Mesh
{
private:
	struct Point
	{
		double x; 
		double y; 
		double z; 
	};
	struct Triangle
	{
		int v1;
		int v2;
		int v3;
	};

	std::vector<Point> vertices;
	std::vector<Triangle> triangleIndexes;

	SDL_Renderer* renderer;
	Point center;

	Parser parser;
	unsigned int scale;

	Point& getNewCoordinate(const Point& p) const
	{
		Point np = { p.x * scale + center.x, center.y - p.y * scale, center.z + p.z * scale };

		return np;
	}
	void drawTriangle(const Point& p1, const Point& p2, const Point& p3) const
	{
		const Point np1 = getNewCoordinate(p1);
		const Point np2 = getNewCoordinate(p2);
		const Point np3 = getNewCoordinate(p3);

		SDL_RenderDrawLine(renderer, np1.x, np1.y, np2.x, np2.y);
		SDL_RenderDrawLine(renderer, np1.x, np1.y, np3.x, np3.y);
		SDL_RenderDrawLine(renderer, np2.x, np2.y, np3.x, np3.y);
	}

public:
	Mesh(SDL_Renderer* renderer, const std::string& filename, int x, int y, unsigned int scale) : renderer(renderer), center{ 1.0 * x, 1.0 * y, 0.0 }, scale(scale), parser(filename)
	{
		unsigned short int count = 0;

		while (true)
		{
			std::vector<float> arr = parser.get();

			if (parser.lst_is_index())
			{
				triangleIndexes.push_back(Triangle{ (int)arr.at(0) - 1, (int)arr.at(1) - 1, (int)arr.at(2) - 1 });
			}
			else if(parser.lst_is_vertice())
			{
				vertices.push_back(Point{ arr.at(0), arr.at(1), arr.at(2) });
			}
			else if (parser.isEnd())
			{
				break;
			}

			parser.reset();
		}

		parser.close();
	}
	void draw() const
	{
		SDL_SetRenderDrawColor(renderer, 125, 155, 180, 255);
		for (const Triangle& triangle : triangleIndexes)
		{
			drawTriangle(vertices.at(triangle.v1), vertices.at(triangle.v2), vertices.at(triangle.v3));
		}
	}

	void rotateY(const double& angle)
	{
		for (Point& p : vertices)
		{
			double x = p.x;

			p.x = cos(angle) * x + p.z * sin(angle);
			p.z = cos(angle) * p.z - x * sin(angle);
		}
	}
	void rotateX(const double& angle)
	{
		for (Point& p : vertices)
		{
			double y = p.y;

			p.y = cos(angle) * y - p.z * sin(angle);
			p.z = cos(angle) * p.z + y * sin(angle);
		}
	}

	void rotateZ(const double& angle)
	{
		for (Point& p : vertices)
		{
			double x = p.x;

			p.x = cos(angle) * x - p.y * sin(angle);
			p.y = cos(angle) * p.y + x * sin(angle);
		}
	}
	void upScale(const int& dif)
	{
		if ((int)scale + dif >= 0)
		{
			scale += dif;
		}
	}
};