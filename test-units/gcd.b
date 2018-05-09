declblock{
  int a,b,temp,t;
}
codeblock{
  t=100000;
  while(t>0){
    a=3;b=9;
    while(b!=0)
    {
      if(a<b){temp=a;a=b;a=temp;}
      temp=b;
      b=a-b*(a/b);
      a=temp;
    }
    println "gcd is ",a;
    t=t-1;

    }
}
