// DEFINE YOUR RUN
#define RUN02 //RUN02

#ifdef RUN01
#include"studentinfo.h"
#include<string>
using std::string;
#include"stack.h"
#include"queue.h"

int main() {



	Stack<std::string> stack;
	Queue<std::string> queue;

	stack.push(StudentInfo::name());
	stack.push("Feynman");
	stack.push("Turing");
	stack.push("Einstein");
	stack.push("Bohr");
	stack.push("Sanchez");

	stack.print();

	queue.enqueue(stack.peek());
	stack.pop();

	queue.enqueue(stack.peek());
	stack.pop();

	queue.enqueue(stack.peek());
	stack.pop();

	queue.enqueue(stack.peek());
	stack.pop();

	queue.dequeue();
	queue.dequeue();

	queue.print();

	return 0;
}

#elif defined(RUN02) //Test the game

#include"game.h"

int main() {


	//Setup game
	Game g("settings.ini");

	//Exectute game loop
	g.run();

	return 0;
}



#endif
