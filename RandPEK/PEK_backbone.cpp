#include<bits/stdc++.h>
using namespace std;
struct atom{
    int x;
    int y;
    int z;
    int type;
};
const double Navogadro=6.022e23;
int a,b,c,chainnum;
long int natom=0;
int box[505][505][505],self[505][505][505];
int rx[2002],ry[2002],rz[2002];
int chain_len[2002];
atom bone[2002][5005];
atom tchain[5005];
int tcount=0;

int xx,yy,zz;
int gen(int* px,int* py, int* pz,int i,int ty)
{
    int x,y,z,updown,direc;
    x=*px;
    y=*py;
    z=*pz;
    if ( ty == -1 ) // 链的第一个原子
    {
        do {
            x=(rand() % (a-2))+1;
            y=(rand() % (b-2))+1;
            z=(rand() % (c-2))+1;
        }while(box[x][y][z]!=0);



    }
    else if ( ty==1 ) //　非苯环虚拟原子
    {
        int t=0;
        do{
            LABEL1:  t++;
            if(t>1200)      //尝试次数过多无法找到空余的相邻晶格点，此链需要重新生成
            {
                tcount++;
                return 0;
            }
            else
            {
                //随机确定下一步的方向
                updown=(rand()%3)-1;//-1为向下，1为向上，0为当前层
                direc=rand()%4; //0,1,2,3
                
                if(updown%2 != 0)//上下走
                {
                    z = *pz + updown;
                    rz[i]=rz[i-1]+updown;
                    if(direc==0)                //rx,ry,rz记录非周期性坐标位置
                    {
                        y+=1;
                        rx[i]=rx[i-1];
                        ry[i]=ry[i-1]+1;
                    }
                    else if(direc==1)
                    {
                        x-=1;
                        rx[i]=rx[i-1]-1;
                        ry[i]=ry[i-1];                        
                    }
                    else if(direc==2)
                    {
                        y-=1;
                        rx[i]=rx[i-1];
                        ry[i]=ry[i-1]-1;                        
                    }
                    else if(direc==3)
                    {
                        x+=1;
                        rx[i]=rx[i-1]+1;
                        ry[i]=ry[i-1];
                    }
                }
                else
                {
                    rz[i]=rz[i-1];
                    if(direc==0)
                    {
                        x+=1;
                        y+=1;
                        rx[i]=rx[i-1]+1;
                        ry[i]=ry[i-1]+1;
                    }
                    else if(direc==1)
                    {
                        x-=1;
                        y+=1;
                        rx[i]=rx[i-1]-1;
                        ry[i]=ry[i-1]+1;                        
                    }
                    else if(direc==2)
                    {
                        x-=1;
                        y-=1;
                        rx[i]=rx[i-1]-1;
                        ry[i]=ry[i-1]-1;
                    }
                    else if(direc==3)
                    {
                        x+=1;
                        y-=1;
                        rx[i]=rx[i-1]+1;
                        ry[i]=ry[i-1]-1;
                    }
                }
                
                //周期性坐标
                if(x>=a)x-=a;
                if(x<0)x+=a;
                if(y>=b)y-=b;
                if(y<0)y+=b;
                if(z>=c)z-=c;
                if(z<0)z+=c;

                //判定生成的坐标是否有重叠
                if (box[x][y][z]==0 && self[x][y][z]==0)
                {
                    break;
                }
                else //有重叠
                {
                    x=*px;
                    y=*py;
                    z=*pz;
                    goto LABEL1;
                }

            }
            
        }while(1);
    }
    else if ( ty ==2 ) //　苯环虚拟原子
    {
        int t=0;       //xx,yy,zz 对应生成苯环末尾原子的坐标
        do{
            LABEL2: t++;
            if(t>1200)      //尝试次数过多无法找到空余的相邻晶格点，此链需要重新生成
            {
                tcount++;
                return 0;
            }
            else
            {
                //随机确定下一步的方向
                updown=(rand()%3)-1;//-1为向下，1为向上，0为当前层
                direc=rand()%4; //0,1,2,3
                

                if(updown%2 != 0)//上下走
                {
                    z = *pz + updown;
                    zz = z + updown;
                    rz[i]=rz[i-1]+updown;
                    rz[i+1]=rz[i]+updown;
                    if(direc==0)                //rx,ry,rz记录非周期性坐标位置
                    {
                        y+=1;
                        rx[i]=rx[i-1];
                        ry[i]=ry[i-1]+1;


                        xx=x;
                        yy=y+1;
                        rx[i+1]=rx[i];
                        ry[i+1]=ry[i]+1;

                    }
                    else if(direc==1)
                    {
                        x-=1;
                        rx[i]=rx[i-1]-1;
                        ry[i]=ry[i-1];

                        xx=x-1;
                        yy=y;
                        rx[i+1]=rx[i]-1;
                        ry[i+1]=ry[i];
                    }
                    else if(direc==2)
                    {
                        y-=1;
                        rx[i]=rx[i-1];
                        ry[i]=ry[i-1]-1;
                        
                        xx=x;
                        yy=y-1;
                        rx[i+1]=rx[i];
                        ry[i+1]=ry[i]-1;
                    }
                    else if(direc==3)
                    {
                        x+=1;
                        rx[i]=rx[i-1]+1;
                        ry[i]=ry[i-1];

                        xx=x+1;
                        yy=y;
                        rx[i+1]=rx[i]+1;
                        ry[i+1]=ry[i];
                    }
                }
                else
                {
                    zz=z;
                    rz[i]=rz[i-1];
                    rz[i+1]=rz[i];
                    if(direc==0)
                    {
                        x+=1;
                        y+=1;
                        rx[i]=rx[i-1]+1;
                        ry[i]=ry[i-1]+1;

                        xx=x+1;
                        yy=y+1;       
                        rx[i+1]=rx[i]+1;
                        ry[i+1]=ry[i]+1;
                    }
                    else if(direc==1)
                    {
                        x-=1;
                        y+=1;
                        rx[i]=rx[i-1]-1;
                        ry[i]=ry[i-1]+1;


                        xx=x-1;
                        yy=y+1;
                        rx[i+1]=rx[i]-1;
                        ry[i+1]=ry[i]+1;                        
                    }
                    else if(direc==2)
                    {
                        x-=1;
                        y-=1;
                        rx[i]=rx[i-1]-1;
                        ry[i]=ry[i-1]-1;

                        xx=x-1;
                        yy=y-1;
                        rx[i+1]=rx[i]-1;
                        ry[i+1]=ry[i]-1;
                    }
                    else if(direc==3)
                    {
                        x+=1;
                        y-=1;
                        rx[i]=rx[i-1]+1;
                        ry[i]=ry[i-1]-1;

                        xx=x+1;
                        yy=y-1;
                        rx[i+1]=rx[i]+1;
                        ry[i+1]=ry[i]-1;
                    }
                }


                //周期性坐标
                if(x>=a)x-=a;
                if(x<0)x+=a;
                if(y>=b)y-=b;
                if(y<0)y+=b;
                if(z>=c)z-=c;
                if(z<0)z+=c;

                if(xx>=a)xx-=a;
                if(xx<0)xx+=a;
                if(yy>=b)yy-=b;
                if(yy<0)yy+=b;
                if(zz>=c)zz-=c;
                if(zz<0)zz+=c;

                if (box[x][y][z]==0 && self[x][y][z]==0 && box[xx][yy][zz]==0 && self[xx][yy][zz]==0)
                {
                    break;
                }
                else //有重叠
                {
                    x=*px;
                    y=*py;
                    z=*pz;
                    goto LABEL2;
                }
            }
        }while(1);
    }
        


    *px=x;
    *py=y;
    *pz=z;
    
    return 1;
}

int main()
{
    int bonds=0,angles=0,dihedrals=0;
    float rho;
    srand(time(0));
    freopen("chain.txt","r",stdin);
    string x;
    getline(cin,x);
    cin>>chainnum;                      //链的总数量

    int length,lennum;
    for (int i=0;i<chainnum;i++)
    {
        cin>>length>>lennum;            //各种链的长度及其数量
        for (int k=0;k<lennum;k++)
        {
            chain_len[i+k]=length;
            if(chain_len[i+k]>=2) bonds+=chain_len[i+k]-1;
            if(chain_len[i+k]>=3) angles+=chain_len[i+k]-2;
            if(chain_len[i+k]>=4) dihedrals+=chain_len[i+k]-3;
        }
        natom+=length*lennum;
        i=i+lennum-1;
    }
    fclose(stdin);
    freopen("CON","r",stdin);
    char ch;
    while(1)
    {
        cout<<"Please input a box size(a,b,c): "<<endl;
        cin>>a>>b>>c;
        rho=natom/Navogadro*14.02/(a*b*c*pow(1.082*1e-8,3));
        cout<<"the density of the box is "<<rho<<"g/cm3"<<endl;
        cout<<"continue?(Y/n)";
        cin>>ch;
        if(ch=='Y'||ch=='y')
        {
            break;
        }
        else continue;
    }

    //定义FCC晶格坐标
    int i,j,k;
    for(i=0;i<a+2;i++)
    {
        for(j=0;j<b+2;j++)
        {
            for(k=0;k<c+2;k++)
            {
                if (k%2==1)
                {
                    box[i][j][k]=(i+j)%2-1;             //非FCC晶格点 -1
                }
                else
                {
                    box[i][j][k]=(i+j+1)%2-1;           //  FCC晶格点 0
                }

            }
        }
    }


    //---------------------------------


    int xpos,ypos,zpos;
    int* px=&xpos;
    int* py=&ypos;
    int* pz=&zpos;
    for(int n=0 ; n < chainnum ; n++)
    {
        cout<<"Generating Chain ..."<<n+1<<endl;

        //重置self[]
        for(int ii=0;ii<a+2;ii++)
        {
            for(int jj=0;jj<b+2;jj++)
            {
                for(int kk=0;kk<c+2;kk++)
                {
                    self[ii][jj][kk]=box[ii][jj][kk];
                }
            }
        }



        for (int i=0;i<chain_len[n];i++)
        {

            ///////////////////////////////

            if ( i==0 )  //生成链的第一个原子
            {
                if ( gen(px,py,pz,0,-1) )
                {
                    rx[0]=xpos;
                    ry[0]=ypos;
                    rz[0]=zpos;
                    self[xpos][ypos][zpos]=1; //自身占据的位置为1
                    tchain[0].x=xpos;
                    tchain[0].y=ypos;
                    tchain[0].z=zpos;
                }
            }
            ////////////////////////////////

            else if ( i%8!=2 && i%8!=6 ) //　非苯环虚拟原子
            {
                int k=gen(px,py,pz,i,1);
                if ( k==0 )                                         //此链需要重新生成
                {
                    i--;
                    continue;
                }
                else if (k==1)                                      //新生成的坐标可用
                {
                    self[xpos][ypos][zpos]=1;  //自身链新增的空间坐标置为1
                    tchain[i].x=xpos;
                    tchain[i].y=ypos;
                    tchain[i].z=zpos;                   
                }
            }
            ///////////////////////////////

            else if ( i%8==2 || i%8==6 ) //生成苯环虚拟原子
            {
                int k=gen(px,py,pz,i,2);
                if ( k==0 )                                         //此链需要重新生成
                {
                    i--;
                    continue;
                }
                else if (k==1)
                {
                    self[xpos][ypos][zpos]=1;  //自身链新增的空间坐标置为1
                    tchain[i].x=xpos;
                    tchain[i].y=ypos;
                    tchain[i].z=zpos;

                    //更新下一个原子
                    i++;

                    xpos=xx;
                    ypos=yy;
                    zpos=zz;
                    self[xpos][ypos][zpos]=1; 
                    tchain[i].x=xpos;
                    tchain[i].y=ypos;
                    tchain[i].z=zpos;

                }
            }

            ////////////////////////////////

        }
        //整链生成完成，更新box
        for(int i=0;i<chain_len[n];i++)
        {
            box[tchain[i].x][tchain[i].y][tchain[i].z]=1;
            bone[n][i].x=rx[i];
            bone[n][i].y=ry[i];
            bone[n][i].z=rz[i];
            rx[i]=0;
            ry[i]=0;
            rz[i]=0;
            int t=i+1;
            if (t%8==1)bone[n][i].type=35;  //sp3_O
            if (t%2==0)bone[n][i].type=8;   //C_R
            if (t%8==3||t%8==7)bone[n][i].type=3;
            if (t%8==5)bone[n][i].type=12;  //C_2

            // if(t%8==1||i%8==3||i%8==5||i%8==7)bone[n][i].type=1;
            // else if(i%8==2||i%8==6)bone[n][i].type=2;
            // else if(i%8==0)bone[n][i].type=3;
            // else bone[n][i].type=4;
        }
    }



 //-----------生成链的实际位置---------
    freopen("skeleton.dat","w",stdout);
    cout<<"# LAMMPS input data file"<<"     "<<tcount<<endl;
    cout<<natom<<"  "<<"atoms"<<endl;
    cout<<bonds<<" "<<"bonds"<<endl;//加和 每条链的长度减一
    cout<<angles<<" "<<"angles"<<endl;
    cout<<dihedrals<<" "<<"dihedrals"<<endl;
    cout<<endl<<"4     atom types"<<endl<<"1     bond types"<<endl<<"1     angle types"<<endl<<"1     dihedral types"<<endl;
    // cout<<"0"<<"    "<<a*1.082<<" xlo xhi"<<endl;
    // cout<<"0"<<"    "<<b*1.082<<" ylo yhi"<<endl;
    // cout<<"0"<<"    "<<c*1.082<<" zlo zhi"<<endl;
    cout<<"0"<<"    "<<a<<" xlo xhi"<<endl;
    cout<<"0"<<"    "<<b<<" ylo yhi"<<endl;
    cout<<"0"<<"    "<<c<<" zlo zhi"<<endl;
    cout<<endl<<"Masses"<<endl<<endl;
    for (int i=1;i<=4;i++)
    {
        cout<<i<<"          14.02"<<endl;
    }
    
    cout<<endl<<"Atoms"<<endl;
    cout<<endl;

    //cout<<chain_length<<endl;
    int num=0;
    for(int n=0;n<chainnum;n++)
    {
        for(int i=0;i<chain_len[n];i++)
        {
            num++;
            //cout<<num<<"    "<<n+1<<"    "<<"1"<<"   "<<bone[n][i].x*1.082<<"    "<<bone[n][i].y*1.082<<"    "<<bone[n][i].z*1.082<<endl;
            cout<<num<<"    "<<n+1<<"    "<<bone[n][i].type<<"   "<<bone[n][i].x<<"    "<<bone[n][i].y<<"    "<<bone[n][i].z<<endl;
        
        }
        //cout<<chain_length[n]<<endl;
    }
    num=0;
    int len=0;
    cout<<endl<<"Bonds"<<endl<<endl;
    for(int n=0;n<chainnum;n++)
    {
        if(n>0)len+=chain_len[n-1];
        for(int i=0;i<chain_len[n]-1;i++)
        {
            num++;
            cout<<num<<"    "<<"1"<<"   "<<i+1+len<<"    "<<i+2+len<<endl;
        }
    }
    cout<<endl<<"Angles"<<endl<<endl;
    num=0;
    len=0;
    for(int n=0;n<chainnum;n++)
    {
        if(n>0)len+=chain_len[n-1];
        for(int i=0;i<chain_len[n]-2;i++)
        {
            num++;
            cout<<num<<"    "<<"1"<<"   "<<i+1+len<<"    "<<i+2+len<<"    "<<i+3+len<<endl;
        }
    }
    cout<<endl<<"Dihedrals"<<endl<<endl;
    num=0;
    len=0;
    for(int n=0;n<chainnum;n++)
    {
        if(n>0)len+=chain_len[n-1];
        for(int i=0;i<chain_len[n]-3;i++)
        {
            num++;
            cout<<num<<"    "<<"1"<<"   "<<i+1+len<<"    "<<i+2+len<<"    "<<i+3+len<<"    "<<i+4+len<<endl;
        }
    }

}