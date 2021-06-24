#!/bin/bash

g++ -c main.cpp -o main.o

g++ main.o -o snake.app -lsfml-graphics -lsfml-window -lsfml-system
