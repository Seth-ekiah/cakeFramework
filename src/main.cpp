#include <iostream>

import ECS;

int main() {
	Entity a;

	a.addComponent<Transform>();
	a.addComponent<TestComp>();

	if (Transform* t = a.getComponent<Transform>())
		std::cout << "Yeah!" << std::endl;
	else
		std::cout << "No!" << std::endl;


	return 0;
}	