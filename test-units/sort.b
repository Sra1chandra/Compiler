declblock{
  int n,a[100],t;
  int i,j,temp;
}
codeblock{
  t=1;
    n=10;
    while(t>0)
    {
    a[0]=2;a[1]=1;a[2]=4;a[3]=0;a[4]=8;a[5]=6;a[6]=4;a[7]=10;a[8]=18;a[9]=2;
    for i=0,n{
        for j= 0,n-i-1{
            if (a[j] < a[j+1]) {
              temp=a[j];
              a[j]=a[j+1];
              a[j+1]=temp;
            }
        }
    }
    for i=0,n{
      println "" , a[i];
    }
    t=t-1;
    }
}
