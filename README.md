# Differentiator

## Main info:
____
- ***OS: Windows 10***
- This program allows you to calculate the derivative of an expression in C.
- To run the program, you need to input data in text file and will get result.
- The program also uses a package of utilities for automatic visualization of graphs specified in the form of a description in the ***DOT and Tex languages***.
____

## To compile:

____

```g++ Diff.cpp Diff_function.cpp recursive_descent.cpp -o Diff.exe ```

- input your equation in ```open.txt``` with a ```$``` sign at the end of the line as a termination
- launch the .exe file and get the result in ```Result.tex```
- if you do not have such packages, but you need the result, you can use third-party sites that read the Tex file, such as https://www.overleaf.com
- also if you have got ***Graphviz package and Tex language*** , then you get graph_1.png with graph and tex file with equation
____

## Example:

___Equation___: 3*sin(5*x)$

___Result in Tex___: 

![Img alt](https://github.com/shaazmik/Differentiator/blob/main/Tex.png)

___Graph___:

![Img alt](https://github.com/shaazmik/Differentiator/blob/main/graph_1.png)
