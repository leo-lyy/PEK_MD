#include<bits/stdc++.h>
using namespace std;
struct atom{
    double x;
    double y;
    double z;
    int type;
    long int id;
};

const long int MAXATOM=2002;
const float pi=3.1415926;
const float r=1.39;     // 苯环键长为1.39埃
const float rco=1.213;  // 羰基碳氧键键长为1.213埃
atom bone[MAXATOM][MAXATOM],benc[MAXATOM][MAXATOM];
long int natom,bonds=0,angles=0,dihedrals=0,impropers=0;
int chain_len[MAXATOM];

// void rotation(double x,double y,double z,double* aa,double* bb,double* cc)
// {
//     double a,b,c;
//     a= *aa;
//     b= *bb;
//     c= *cc;
//     //double rot = rand()%180 *pi/180;
//     double rot = pi/4;
//     double ccos = cos(rot);
//     double ssin = sin(rot);
//     double nx = (a*a*(1-ccos)+ccos)*x+(a*b*(1-ccos)-c*ssin)*y+(a*c*(1-ccos)+b*ssin)*z;
//     double ny = (b*c*(1-ccos)+c*ssin)*x+(b*b*(1-ccos)*ccos)*y+(b*c*(1-ccos)-a*ssin)*z;
//     double nz = (a*c*(1-ccos)-b*ssin)*x+(b*c*(1-ccos)*x*ssin)*y+(c*c*(1-ccos)+ccos)*z;
//     *aa=nx;
//     *bb=ny;
//     *cc=nz;

// }

int main()
{
    srand(time(NULL)); // 初始化随机数种子
    int k,molatom=1,chainnum,id=0;
    float xlo,xhi,ylo,yhi,zlo,zhi;
    double vecx,vecy,vecz,mx,my,mz;
    //read initial data from "xxx.dat"
    memset(chain_len,0,sizeof(chain_len));
    ifstream infile("fcc_unwrapped_coordinate.dat");
    if (infile.is_open())
    {
        string xx;
        getline(infile,xx);
        infile>>natom;
        for(int i =0;i<9;i++)getline(infile,xx);
        infile>>xlo>>xhi;
        getline(infile,xx);
        infile>>ylo>>yhi;
        getline(infile,xx);
        infile>>zlo>>zhi;
        getline(infile,xx);
        for(int i =0;i<10;i++)
        {
            getline(infile,xx); 
        }       
        for(long int i = 1;i<=natom;i++)
        {
            
            infile>>k>>k;
            if(id!=k)   // id 用于记录上一个原子的分子链序号，data文件的Atoms数据中第二列为分子链序号，顺序排列
            {           //  对比 id 和 k, 得知是否开始输入新的分子链
                id=k;
                molatom=1;
                bonds+=16;

            }
            else
            {
                molatom++;

            }
            infile>>bone[k][molatom].type>>bone[k][molatom].x>>bone[k][molatom].y>>bone[k][molatom].z;
            chain_len[k]++;
            if(molatom%8==0)
            {
                bonds+=17;
                angles+=24;
                dihedrals+=32;
                impropers+=5;
            }
            
            //if(bone[k][molatom].type==3)natom--;
        }
        chainnum=k;
        bonds-=chainnum*17;
        angles-=chainnum*3;
        dihedrals-=chainnum*6;
        impropers-=chainnum;

    }        
    else
    {
        cout <<"failed to open the file"<<endl;
    }
    infile.close();

    k=0;
    for(int i=1;i<=chainnum;i++)
    {
        int bc=1;
        for (int j=1;j<=chain_len[i];j++)
        {
            k++;
            if (j%8==3 || j%8==7) //遍历到苯环中心虚拟原子
            {
                
                //获得苯环头原子到虚拟原子的方向矢量
                vecx=bone[i][j].x-bone[i][j-1].x;
                vecy=bone[i][j].y-bone[i][j-1].y;
                vecz=bone[i][j].z-bone[i][j-1].z; 
                
                //获得苯环头原子和中心虚拟原子的坐标中点
                mx=(bone[i][j].x+bone[i][j-1].x)/2;
                my=(bone[i][j].y+bone[i][j-1].y)/2;
                mz=(bone[i][j].z+bone[i][j-1].z)/2;


                // 已知一平面的方向向量为vec(x,y,z)，平面上有一点m(x,y,z)，
                // 如何随机生成一点 B,使得 B 在这个平面上，且到点 A 的距离为r=1.39
                // (实现方法来源于ChatGPT-3.5-Turbo)


                // 计算平面方程系数
                double D = -vecx*mx - vecy*my - vecz*mz;
                double A = vecx;
                double B = vecy;
                double C = vecz;

                if (C==0)C=0.00001; //此处ChatGPT给出的方案未考虑z方向上的向量为0，这会导致后续出现 /0 的错误
                                    //稍微改变下z值就可以解决

                // 随机生成点B
                double s = (double)rand() / RAND_MAX; // 生成[0,1)之间的随机数
                double t1 = (double)rand() / RAND_MAX;
                double x0 = s;
                double y0 = t1;
                double z0 = (-A*x0 - B*y0 - D) / C;
                    
                // 将点B沿着从点A到点B的向量方向平移t的距离
                double dx = x0 - mx;
                double dy = y0 - my;
                double dz = z0 - mz;
                double len = sqrt(dx*dx + dy*dy + dz*dz);
                double scale = r / len;
                double xb = mx + dx*scale;
                double yb = my + dy*scale;
                double zb = mz + dz*scale;
                //最终 B点坐标为(xb,yb,zb)
                
                ////////// GPT YYDS!!!

                //上述方法生成的坐标再随机旋转
                double* bx=&xb;
                double* by=&yb;
                double* bz=&zb;
                // vecx=vecx/sqrt(vecx*vecx+vecy*vecy+vecz*vecz);
                // vecy=vecy/sqrt(vecx*vecx+vecy*vecy+vecz*vecz);
                // vecz=vecz/sqrt(vecx*vecx+vecy*vecy+vecz*vecz);
                // rotation(vecx,vecy,vecz,bx,by,bz);
                
                benc[i][bc].x=*bx;
                benc[i][bc].y=*by;
                benc[i][bc].z=*bz;
                // 再根据对称关系，生成苯环上另外三个原子
                benc[i][bc+1].x=mx-benc[i][bc].x+mx;
                benc[i][bc+1].y=my-benc[i][bc].y+my;
                benc[i][bc+1].z=mz-benc[i][bc].z+mz;

                benc[i][bc+2].x=bone[i][j].x-benc[i][bc].x+bone[i][j].x;
                benc[i][bc+2].y=bone[i][j].y-benc[i][bc].y+bone[i][j].y;
                benc[i][bc+2].z=bone[i][j].z-benc[i][bc].z+bone[i][j].z;

                benc[i][bc+3].x=bone[i][j].x-benc[i][bc+1].x+bone[i][j].x;
                benc[i][bc+3].y=bone[i][j].y-benc[i][bc+1].y+bone[i][j].y;
                benc[i][bc+3].z=bone[i][j].z-benc[i][bc+1].z+bone[i][j].z;

                benc[i][bc].type=5;
                benc[i][bc+1].type=5;
                benc[i][bc+2].type=5;
                benc[i][bc+3].type=5;
                bc+=4;

                natom+=3;
            }
            else if(j%8==5) //遍历到C=O, 生成羰基上的氧原子
            {
                // 利用上述生成苯环碳原子类似的方法

                //获得羰基碳原子到前一个原子（苯环末尾原子）的方向矢量
                vecx=bone[i][j].x-bone[i][j-1].x;
                vecy=bone[i][j].y-bone[i][j-1].y;
                vecz=bone[i][j].z-bone[i][j-1].z; 

                //平面中心原子坐标，即羰基碳原子的坐标
                mx=bone[i][j].x;
                my=bone[i][j].y;
                mz=bone[i][j].z;
                // 计算平面方程系数
                double D = -vecx*mx - vecy*my - vecz*mz;
                double A = vecx;
                double B = vecy;
                double C = vecz;
                
                // 随机生成点B
                if (C==0)C=0.00001;
                double s = (double)rand() / RAND_MAX; // 生成[0,1)之间的随机数
                double t1 = (double)rand() / RAND_MAX;
                double x0 = s;
                double y0 = t1;
                double z0 = (-A*x0 - B*y0 - D) / C;
                // 将点B沿着从点A到点B的向量方向平移t的距离
                double dx = x0 - mx;
                double dy = y0 - my;
                double dz = z0 - mz;
                double len = sqrt(dx*dx + dy*dy + dz*dz);
                double scale = r / len;
                double xb = mx + dx*scale;
                double yb = my + dy*scale;
                double zb = mz + dz*scale;
                //最终 P点坐标为(xb,yb,zb)


                benc[i][bc].x=xb;
                benc[i][bc].y=yb;
                benc[i][bc].z=zb;
                benc[i][bc].type=6;
                bc++;

                natom++;
            }
        
        }

    }
    ofstream outfile("unfolded.data"); 
    outfile<<"# LAMMPS input data file"<<endl; 
    outfile<<natom<<"       atoms"<<endl;
    outfile<<bonds<<"       bonds"<<endl;
    outfile<<angles<<"      angles"<<endl;
    // outfile<<angles<<"       angles"<<endl;
    // outfile<<dihedrals<<"       dihedrals"<<endl;
    // outfile<<impropers<<"       impropers"<<endl;

    outfile<<" 6 atom types"<<endl;
    outfile<<" 4 bond types"<<endl;
    outfile<<" 3 angle types"<<endl;

    outfile<<endl;
    outfile<<xlo<<"     "<<xhi<<"       xlo     xhi"<<endl;
    outfile<<ylo<<"     "<<yhi<<"       ylo     yhi"<<endl;
    outfile<<zlo<<"     "<<zhi<<"       zlo     zhi"<<endl;
    


    outfile<<endl<<"Atoms # molecular"<<endl<<endl;
    long int count=0;
    for(int i=1;i<=chainnum;i++)
    {
        int pos=1;
        //outfile<<endl<<chain_len[i]<<endl;
        for(int j=1;j<=chain_len[i]+1;j++)
        {
            
            int k=bone[i][j].type;
            if (k==1)
            {
                outfile<<++count<<"       "<<i<<"       "<<bone[i][j].type<<"     "<<bone[i][j].x<<"      "<<bone[i][j].y<<"      "<<bone[i][j].z<<endl;
                bone[i][j].id=count;
            }
            else if (k==2)
            {
                outfile<<++count<<"       "<<i<<"       "<<bone[i][j].type<<"       "<<bone[i][j].x<<"      "<<bone[i][j].y<<"      "<<bone[i][j].z<<endl;
                bone[i][j].id=count;
            }
            else if (k==3)
            {
                for(int t=0;t<4;t++)
                {
                    outfile<<++count<<"     "<<i<<"     "<<benc[i][pos+t].type<<"       "<<benc[i][pos+t].x<<"      "<<benc[i][pos+t].y<<"      "<<benc[i][pos+t].z<<endl;
                    benc[i][pos+t].id=count;
                }
                pos+=4;
            }
            else if (k==4)
            {   
                outfile<<++count<<"       "<<i<<"       "<<bone[i][j].type<<"       "<<bone[i][j].x<<"      "<<bone[i][j].y<<"      "<<bone[i][j].z<<endl;
                bone[i][j].id=count;
                outfile<<++count<<"       "<<i<<"       "<<benc[i][pos].type<<"     "<<benc[i][pos].x<<"      "<<benc[i][pos].y<<"      "<<benc[i][pos].z<<endl;
                benc[i][pos].id=count;
                pos++;
            }
        }
    }





    outfile<<endl<<"Bonds"<<endl<<endl;
    count=0;
    for(int i=1;i<=chainnum;i++)
    {
        int pos=0;
        //outfile<<endl<<chain_len[i]<<endl;
        for(int j=1;j<=chain_len[i];j++)
        {
            
            if (j%8==0 && j!=chain_len[i])
            {
                outfile<<++count<<"     "<<"1"<<"       "<<bone[i][j].id<<"        "<<bone[i][j+1].id<<endl;       // C-O
            }
            else if (j%8==1)
            {
                outfile<<++count<<"     "<<"1"<<"       "<<bone[i][j].id<<"        "<<bone[i][j+1].id<<endl;       // O-C
            }
            else if (j%8==2 || j%8==6)
            {
                outfile<<++count<<"     "<<"2"<<"       "<<bone[i][j].id<<"        "<<benc[i][++pos].id<<endl;     // C-C (benzene)
                outfile<<++count<<"     "<<"2"<<"       "<<bone[i][j].id<<"        "<<benc[i][++pos].id<<endl;
                outfile<<++count<<"     "<<"2"<<"       "<<benc[i][pos-1].id<<"        "<<benc[i][pos+2].id<<endl;
                outfile<<++count<<"     "<<"2"<<"       "<<benc[i][pos].id<<"        "<<benc[i][++pos].id<<endl;
                outfile<<++count<<"     "<<"2"<<"       "<<benc[i][pos].id<<"        "<<bone[i][j+2].id<<endl;
                outfile<<++count<<"     "<<"2"<<"       "<<benc[i][++pos].id<<"        "<<bone[i][j+2].id<<endl;
                j++;
            }
            else if (j%8==4)
            {
                // outfile<<++count<<"     "<<"2"<<"       "<<benc[i][pos-1].id<<"         "<<bone[i][j].id<<endl;
                // outfile<<++count<<"     "<<"2"<<"       "<<benc[i][pos].id<<"       "<<bone[i][j].id<<endl;
                outfile<<++count<<"     "<<"3"<<"       "<<bone[i][j].id<<"        "<<bone[i][j+1].id<<endl;       // C-C
            }
            else if (j%8==5)
            {
                outfile<<++count<<"     "<<"4"<<"       "<<bone[i][j].id<<"        "<<benc[i][++pos].id<<endl;     // C=O
                outfile<<++count<<"     "<<"3"<<"       "<<bone[i][j].id<<"        "<<bone[i][j+1].id<<endl;       // C-C
            }


        }
    }


    outfile<<endl<<"Angles"<<endl<<endl;
    count=0;
    for (int i=1;i<=chainnum;i++)
    {
        int pos=0;
        for(int j=1;j<=chain_len[i];j++)
        {
            if( j%8 == 0  &&  j != chain_len[i] )
            {
                outfile<<++count<<"     "<<"3"<<"       "<<bone[i][j].id<<"         "<<bone[i][j+1].id<<"       "<<bone[i][j+2].id<<endl;
            }
            else if(j%8==2 || j%8==6)
            {
                outfile<<++count<<"     "<<"1"<<"       "<<bone[i][j-1].id<<"         "<<bone[i][j].id<<"       "<<benc[i][++pos].id<<endl;
                outfile<<++count<<"     "<<"1"<<"       "<<bone[i][j-1].id<<"         "<<bone[i][j].id<<"       "<<benc[i][pos+1].id<<endl;
                outfile<<++count<<"     "<<"1"<<"       "<<benc[i][pos].id<<"         "<<bone[i][j].id<<"       "<<benc[i][pos+1].id<<endl;
                outfile<<++count<<"     "<<"1"<<"       "<<bone[i][j].id<<"         "<<benc[i][pos].id<<"       "<<benc[i][pos+3].id<<endl;
                outfile<<++count<<"     "<<"1"<<"       "<<bone[i][j].id<<"         "<<benc[i][pos+1].id<<"       "<<benc[i][pos+2].id<<endl;
                outfile<<++count<<"     "<<"1"<<"       "<<benc[i][pos].id<<"         "<<benc[i][pos+3].id<<"       "<<bone[i][j+2].id<<endl;
                outfile<<++count<<"     "<<"1"<<"       "<<benc[i][pos+1].id<<"         "<<benc[i][pos+2].id<<"       "<<bone[i][j+2].id<<endl;
                outfile<<++count<<"     "<<"1"<<"       "<<benc[i][pos+3].id<<"         "<<bone[i][j+2].id<<"       "<<benc[i][pos+3].id<<endl;
                if(j!=chain_len[i]-2)
                {
                    outfile<<++count<<"     "<<"1"<<"       "<<benc[i][pos+3].id<<"         "<<bone[i][j+2].id<<"       "<<bone[i][j+3].id<<endl;
                    outfile<<++count<<"     "<<"1"<<"       "<<benc[i][pos+1].id<<"         "<<bone[i][j+2].id<<"       "<<bone[i][j+3].id<<endl;
                }
                
                j+=1;
                pos+=3;
            }
            else if (j%8==5)
            {
                outfile<<++count<<"     "<<"2"<<"       "<<bone[i][j-1].id<<"         "<<bone[i][j].id<<"       "<<bone[i][j+1].id<<endl;
                outfile<<++count<<"     "<<"2"<<"       "<<bone[i][j-1].id<<"         "<<bone[i][j].id<<"       "<<benc[i][++pos].id<<endl;
                outfile<<++count<<"     "<<"2"<<"       "<<bone[i][j+1].id<<"         "<<bone[i][j].id<<"       "<<benc[i][pos].id<<endl;
            }
        }
    }
        /// To be finished... 
    


    outfile.close();
}