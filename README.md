1. Data Types

Inegers and Array of Integers.

int data, array[100];
int sum;

All the variables have to be declared in the declblock{....} before being used
in the codeblock{...}. Multiple variables can be declared in the statement
and each declaration statement ends with a semi-colon.

2. Expressions

3. for loop

for i = 1, 100 {
	.....
}

for i = 1, 100, 2 {
	.....
}

4. if-else statement

	if expression {
	....
	}
	....

	if expression {
	...
	}
	else {
	....
	}

5. while statment

	while expression {

	}

6. conditional and unconditional goto

	goto label ;

	goto label if expression ;

7. print/read

	print "blah...blah", val
	println "new line at the end"
	read sum
	read data[i]
