#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <iostream>
#include <SDL2/SDL.h>

template <typename T>
void fatalError(T t){
	std::cout << t << " ";
}

template<typename T, typename... Args>
void fatalError(T t, Args... args){
	std::cout << t << " ";

	fatalError(args...);

	std::cout << std::endl;

	system("PAUSE");

	SDL_Quit();
	exit(-1);
}

template<typename T>
void normalError(T t) {
	std::cout << t << " ";
}

template<typename T, typename... Args>
void normalError(T t, Args... args) {
	std::cout << t << " ";

	normalError(args...);

	std::cout << std::endl;
}

#endif
