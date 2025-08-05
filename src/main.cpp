#include <iostream>

import ECS;

int main() {
	Entity a;

	if (Transform* t = a.addComponent<Transform>())
		// std::cout << "success" << std::endl;
		t->Test();
	else
		std::cout << "fail" << std::endl;

	// a.removeComponent<Transform>();



	return 0;
}	