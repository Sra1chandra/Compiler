declblock{
	int a,b;
	int c[10];
}
codeblock{
L3:	b=2;
	if 1==2 || 1==1 {b=0;}else{b=4;}
	for a=b,b+3,1{println "",a;}
	while b<10{
	L1:println "ss",b;
	b=b+1;
	goto L1 if b<2;
	b=b+2;
	}
	goto L3;
L2:	c[0]=3;
	c[1]=b+c[0];
	println "aaaaaaaaaaa";
	println "hello sravan",c[0],c[1],b;
	println "hello chandra",c[3];
	println "sravan chandra" ;
}
