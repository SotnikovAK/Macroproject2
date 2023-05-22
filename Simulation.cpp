#include "Simulation.h"
#include "Quaternion.h"

const sf::Vector2i size(screenwidth, screenheight);

Simulation::Simulation(int n)
{
    CreationElements(UpdateFrames,n);
}

void Simulation::WindowInteraction()
{
    sf::RenderWindow window(sf::VideoMode(WindowSize[0], WindowSize[1]), "Simulation");
    sf::Texture T;
    sf::Event event;
    T.create(window.getSize().x, window.getSize().y);

	window.setMouseCursorVisible(false);

	Quaternion camerapos(0, 0, 0, 0);
	Quaternion cameravel(0, 0, 0, 0);
	Quaternion camerarotation(Quaternion(1, 0, 0, 0).normalized());
	Quaternion temprotation(0, 0, 0, 0);

	int oldMouseX = WindowSize[0] / 2;
	int oldMouseY = WindowSize[1] / 2;
	int dMouseX;
	int dMouseY;

    long long int  t = 0;
    while (window.isOpen())
    {


        while (window.pollEvent(event) )
        {
			if (event.type == sf::Event::Closed or sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) 
			{
				window.close();
			}

			if (event.type == sf::Event::MouseMoved) {
				dMouseX = event.mouseMove.x - oldMouseX;
				dMouseY = event.mouseMove.y - oldMouseY;

				if ((dMouseX != 0) or (dMouseY != 0)) {

					temprotation = Quaternion(1, 0.001 * dMouseY, -0.001 * dMouseX, 0).normalized();
					camerarotation = temprotation * camerarotation;

					sf::Mouse::setPosition(sf::Vector2i(screenwidth / 2, screenheight / 2), window);

					oldMouseX = screenwidth / 2;
					oldMouseY = screenheight / 2;
				}


			}
        }


        for (auto element = ElementList.begin(); element != ElementList.end(); element++)
        {
            element->Starting_conditions_for_W();
        }
        for (auto element = ElementList.begin(); element != ElementList.end(); ++element)
        {
            for (auto force = std::next(element); force != ElementList.end(); force++)
            {
                element->GravitationalForce(&(*force));
            }
        }
        window.clear();

        for (auto element = ElementList.begin(); element != ElementList.end(); element++)
        {

            auto element_ = std::next(element);

            while (element_ != ElementList.end())
            {
                if (element->ClashReporter(*element_))
                {
                    element->InelasticClashForce(&(*element_));
                    element_ = ElementList.erase(element_);
                    element_--;
                }
                element_++;
            }
        }

        for (auto element = ElementList.begin(); element != ElementList.end(); element++)
        {
            ///element->Cout();
            element->Move();

        }



		camerarotation.normalize();

		cameravel = cameravel + camerarotation.inverse() * (Quaternion(0, 0, 0, 0.01 * (double)sf::Keyboard::isKeyPressed(sf::Keyboard::W)) * camerarotation);
		cameravel = cameravel + camerarotation.inverse() * (Quaternion(0, 0, 0, -0.01 * (double)sf::Keyboard::isKeyPressed(sf::Keyboard::S)) * camerarotation);

		camerapos = camerapos + cameravel;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			camerarotation = Quaternion(1, 0, 0, 0.01).normalized() * camerarotation;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
			camerarotation = Quaternion(1, 0, 0, -0.01).normalized() * camerarotation;
		}




        for (auto element = ElementList.begin(); element != ElementList.end(); element++)
        {
            element->CoordFor3D(camerapos, camerarotation);
        }

        for (auto element = ElementList.begin(); element != ElementList.end(); element++)
        {
            ///std::cout << "**" << std::endl;
            element->draw(window);
        }
        /// <summary>
        /// �������� �� ������
        /// </summary>
		/// 
        if (t % UpdateFrames == 0)
        {
            T.update(window);
        }
        ++t;

        window.display();

       
    }
}

void Simulation::CreationElements(float UpdateFrames, int N)
{

    std::seed_seq seed_{ time(NULL) };
    std::mt19937 mt(seed_);
    std::uniform_real_distribution<> X(SpawnBorderEpsillon[0], WindowSize[0] - SpawnBorderEpsillon[0]);
    std::uniform_real_distribution<> Y(SpawnBorderEpsillon[1], WindowSize[1] - SpawnBorderEpsillon[1]);
	std::uniform_real_distribution<> Z(SpawnBorderEpsillon[1], WindowSize[1] - SpawnBorderEpsillon[1]);
    std::uniform_real_distribution<> Vx(0, 1);
    std::uniform_real_distribution<> Vy(0, 1);
	std::uniform_real_distribution<> Vz(0, 1);
    std::uniform_real_distribution<> M(10, 250);

    for (int i = 0; i < N; ++i) {
        ElementList.push_back(ElementarElement(X(mt), Y(mt), Z(mt), UpdateFrames));

    }
    for (auto element = ElementList.begin(); element != ElementList.end(); ++element) {

        element->Starting_conditions_for_V(Vx(mt), Vy(mt), Vz(mt));
        element->Starting_conditions_for_M(M(mt));

    }
}

