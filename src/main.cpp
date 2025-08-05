#include <iostream>

import ECS;

int main() {
	Entity a;

	if (Transform* t = a.addComponent<Transform>())
		t->Test();
	else
		std::cout << "fail" << std::endl;


	a.removeComponent<Transform>();

	
	if (Transform* t = a.addComponent<Transform>())
		t->Test();
	else
		std::cout << "fail" << std::endl;



	return 0;
}	