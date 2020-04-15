#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#define OK 1
#define ERROR 0
#define MVNum 13
#define LEN(x) sizeof(x)/sizeof(x[0])
#define ALL 16

typedef int Status;
typedef int VerTexType;
//save net
typedef struct ArcNode{
    char word[2];
    int adjvex;
    struct ArcNode *nextarc;
}ArcNode;
typedef struct VNode{
    VerTexType data;
    ArcNode *firstarc;
}VNode,AdjList[MVNum];
typedef struct{
    AdjList vextices;
}ALGraph;

//save point
typedef struct{
    int id;
    int area;
    char name[2];
    int left_or_right;
    int Lock;
    int gain;
    int used;
    int part;
    int AorB;
}Point;
Point point[ALL];

typedef struct recordNode{
    char word[2];
    int LR;
    int TB;
    struct recordNode *nextarc;
}recordNode;
typedef struct PNode{
    char data[2];
    int LR;
    int TB;
    recordNode *firstarc;
}PNode,PointList[ALL];
typedef struct{
    PointList points;
}PointGraph;
PointGraph P;

int count;
int TotalGain = 0;
int maxAreaV = 0;
int randomNUm = 0;
int LEFT;
int Right;
int theMaxPart = 0;

int theLengthOfCurrentNetList = 13;
/*........................*/
int key = 0;
void createPointStructure(Point *point, char input[10]){
    int num;
    char *result = NULL;
    result = (char*)malloc(sizeof(char*));
    result = strtok(input, " " );
    //可以用随机数给分配
//    int randomLR = rand()%2;
//    if(randomLR == 1){
//        LEFT++;
//    }else{
//        Right++;
//    }
    //printf("randomNUM : %d \n" , randomLR);
    
    //继续调用strtok，分解剩下的字符串
    strcpy(point->name,result);
    result=strtok(NULL," ");
    //转int
    num = atoi (result);
    //存一下最大点
    if(num>maxAreaV){
        maxAreaV = num;
    }
    result=strtok(NULL," ");
    int LR = atoi(result);
    if(LR == 1){
            LEFT++;
        }else{
            Right++;
    }
    point->left_or_right = LR;
    point->area = num;
    point->Lock = 0;
    point->part = 0;
    printf("%s de LR is %d \n", point->name, LR);

}

//创建邻接表结构
int idx = -1;
Status CreateUDG(ALGraph *G, char input[10])
{
    ArcNode *p1, *p,*p2;
    char *result = NULL;
    result = (char*)malloc(sizeof(char*));
    char *ret;
    ret = strrchr(input, 's');
    //切割
    result = strtok( input, " " );
    //create
    if(ret!=NULL){
        idx++;
        G->vextices[idx].data = idx;
        G->vextices[idx].firstarc = NULL;
        p = (ArcNode *)malloc(sizeof(ArcNode));
        strcpy(p->word, result);
        
        p->nextarc = G->vextices[idx].firstarc;
        G->vextices[idx].firstarc = p;
        
    }else{
        p2 = G->vextices[idx].firstarc;
        p1 = (ArcNode *)malloc(sizeof(ArcNode));
        strcpy(p1->word, result);
        while(p2 != NULL){
            if(p2->nextarc != NULL){
                p2 = p2->nextarc;
            }
            else{
                p2->nextarc = p1;
                p2->nextarc->nextarc = NULL;
                break;
            }
        }
        
//        p1->nextarc = G->vextices[idx].firstarc;
//        G->vextices[idx].firstarc = p1;
    }
    count = count + 1;
    
    return OK;
}

int oriareaL = 0;
int oriareaR = 0;
int areaV = 0;
int minAreaA,maxAreaA,area_result;
void calOriAreaAandB(){
    float ratio = 0.01;
    for(int u = 0;u<ALL;u++){
        areaV = areaV + point[u].area;
        if(point[u].left_or_right == 1){
            oriareaL = oriareaL + point[u].area;
        }else if(point[u].left_or_right == 0){
            oriareaR = oriareaR + point[u].area;
        }
    }
    ratio = (float)oriareaL / (oriareaL + oriareaR);
    minAreaA = (ratio * areaV) - maxAreaV;
    maxAreaA = (ratio * areaV) + maxAreaV;
}

int judgeAreaAAfterMove(int p){
    int LR = point[p].left_or_right;
    int areaPoint = point[p].area;
    int REoriareaL = 0;
    if(LR == 1){
        REoriareaL = oriareaL - areaPoint;
    }else{
        REoriareaL = oriareaL + areaPoint;

    }
    if(REoriareaL<minAreaA || REoriareaL>maxAreaA){
        //unsatisfy Area constrain
        area_result = 0;
    }else{
        //satisfy Area constrain
        area_result = 1;
        oriareaL = REoriareaL;
    }

    return area_result;
}

//point num constrain
int point_result;
int judgePointAfterMove(int p){
    int LR = point[p].left_or_right;
    if(LR == 1){
        LEFT = LEFT-1;
        Right = Right+1;
    }else{
        Right = Right - 1;
        LEFT = LEFT+1;

    }
    if(LEFT-Right>2||LEFT-Right<-2){
        //unsatisfy PointNum constrain
        point_result = 0;
        if(LR == 1){
            Right = Right -1;
            LEFT = LEFT + 1;
        }else{
            
            LEFT = LEFT-1;
            Right = Right+1;

        }
        
    }else{
        if(LR == 1){
            Right = Right -1;
            LEFT = LEFT + 1;
        }else{
            
            LEFT = LEFT-1;
            Right = Right+1;

        }
        //satisfy PointNum constrain
        point_result = 1;
    }
    return point_result;
}

void netFileRead(ALGraph *G){
    FILE *fp;
    char input[10];
    fp = fopen("/Users/jiang/Desktop/xiaojiang 2.txt","r");
    if(fp == NULL)
    {
        printf("Open filefailure!");
        exit(1);
    }
    else
    {
        while(fgets(input, 10, fp) != NULL){
//            if(strrchr(input, 'p') != NULL){
//                continue;
//            }else{
//
//            }
             CreateUDG(G, input);
        }
        fclose(fp);
    }
}

void areFileRead(){
    FILE *fp;
    char input[10];
    fp = fopen("/Users/jiang/Desktop/xiaojiangare 2.txt","r");
    if(fp == NULL)
    {
        printf("Open filefailure!");
        exit(1);
    }
    else
    {
        while(fgets(input, 10, fp) != NULL){
//            if(strrchr(input, 'p')!=NULL){
//                continue;
//            }else{
//
//            }
            point[key].id = key;
            createPointStructure(&point[key], input);
            key++;
        }
        fclose(fp);
    }
}

int calculateGain(ALGraph *G, char* content)
{
   ArcNode *p;
   int q;
   int FS = 0,TS = 0;
   bool isCellInsideGraph = false;
    //
   for (q=0;q<theLengthOfCurrentNetList;q++){
       p = G->vextices[q].firstarc;
       if(p->nextarc == NULL){
           continue;
       }
       int mid = 0;
       int count2 = 0;
       int count = 0;
       bool isCellInsideNet = false;
       while(p!=NULL){
           count2++;
           int middle = 0;
           //
           for(int idex = 0; idex<16; idex++)
           {
               if(strcmp(point[idex].name,content) == 0){
                   mid = point[idex].left_or_right;
               }
               if(strcmp(point[idex].name,p->word) == 0){
                   middle = point[idex].left_or_right;

               }
           }
           count = count + middle;
           if(strcmp(p->word, content)==0){
               isCellInsideNet = true;
               isCellInsideGraph = true;
               //mid = L_or_R(content) ;
           }
           p=p->nextarc;
       }
       if(mid==1 && count == 1 && isCellInsideNet==true){
           FS = FS + 1;
       }else if(mid==0 && count == count2 - 1 && isCellInsideNet==true){
           FS = FS + 1;
       }
       if(count == count2 &&  isCellInsideNet==true){
           TS = TS + 1;
       }else if(count == 0 && isCellInsideNet==true){
           TS = TS + 1;
       }
   }
    if(isCellInsideGraph == true){
        return FS - TS;
    }
    return -999;
}

int thepointNumYouMove = 0;
int calMaxGainOfPart(ALGraph *G, int part){
    int maxGain = -9988;
    char* nextChange;
    int i;
    //
    for(i = 0; i < 16;i++){
        if(point[i].Lock == 1 || point[i].part!=part){
            continue;
        }
        
        int gaga = calculateGain(G, point[i].name);
        if(gaga == -999){
            continue;
        }
        point[i].gain = gaga;
        point[i].used = 0;
        //printf("Cell %s ‘s gain is: %d \n",point[i].name, point[i].gain);
        if(point[i].gain > maxGain){
            nextChange = point[i].name;
            maxGain = point[i].gain;
            thepointNumYouMove = i;
        }
    }
    return maxGain;
}

int findAnotherPointInPart(int num, int part){
    int another = -1;
    while(num>-3){
        for(int ii = 0; ii<16; ii++){
            if(point[ii].used == 1 || point[ii].Lock == 1 || point[ii].part!=part){
                continue;
            }else{
                if(point[ii].gain == num){
                    another = ii;
                    return another;
                }
            }
        }
        num--;
    }
    return another;
}

void movePoint(int thepointNumYouMove){
    printf("now we want to move %s \n", point[thepointNumYouMove].name);
    //swap point
    if(point[thepointNumYouMove].left_or_right == 1){
        point[thepointNumYouMove].left_or_right = 0;
        Right = Right +1;
        LEFT = LEFT - 1;
    }else{
        LEFT = LEFT+1;
        Right = Right-1;

        point[thepointNumYouMove].left_or_right = 1;
    }
    point[thepointNumYouMove].Lock = 1;
    TotalGain = TotalGain + point[thepointNumYouMove].gain;
    printf("Lock Cell :%s \n",point[thepointNumYouMove].name);
    
}

void moveBackPoint(int thepointNumYouMove){
    printf("now we want to move %s \n", point[thepointNumYouMove].name);
    //swap point
    if(point[thepointNumYouMove].left_or_right == 1){
        point[thepointNumYouMove].left_or_right = 0;
        LEFT = LEFT - 1;
        Right = Right + 1;
    }else{
        point[thepointNumYouMove].left_or_right = 1;
        LEFT = LEFT + 1;
        Right = Right - 1;
    }
    point[thepointNumYouMove].Lock = 1;
    TotalGain = TotalGain + point[thepointNumYouMove].gain;
    printf("Lock Cell :%s \n",point[thepointNumYouMove].name);
    
}

int getLorR(char* pointname){
    int LR = 2;
    for(int y = 0; y < ALL; y++){
        if(strcmp(point[y].name, pointname) == 0){
            LR = point[y].left_or_right;
            break;
        }
    }
    return LR;
    
}


int calCutsize(ALGraph *G){
    ArcNode *pointroot, *pointchild;
    int cutsize=0;

    int PointPart,PointNextPart = 2;
    for(int x = 0; x<theLengthOfCurrentNetList; x++){
        pointroot = G->vextices[x].firstarc;
        PointPart = getLorR(pointroot->word);
        pointchild = pointroot->nextarc;
        while (pointchild!=NULL){
            PointNextPart = getLorR(pointchild->word);
            if(PointPart!=PointNextPart){
                cutsize = cutsize + 1;
            }
            pointchild = pointchild->nextarc;
        }
        
        
    }
    
    return cutsize;
}

void sepNetList(ALGraph *G){
    ArcNode *point1, *point2, *point3 = NULL;
    int PointPart1,PointNextPart1 = 2;
    recordNode *r, *s;
    for(int x = 0; x<theLengthOfCurrentNetList; x++){
        point1 = G->vextices[x].firstarc;
        point3 = point1;
        PointPart1 = getLorR(point1->word);
        point2 = point1->nextarc;
        while (point2!=NULL){
            PointNextPart1 = getLorR(point2->word);
            if(PointPart1!=PointNextPart1){
                printf("cut point%s --- point%s\n",point1->word,point2->word);
                r = (recordNode *)malloc(sizeof(recordNode));
                s = (recordNode *)malloc(sizeof(recordNode));
                for(int i=0; i<ALL;i++){
                    if(strcmp(P.points[i].data,point1->word)==0){
                        strcpy(r->word, point2->word);
                        r->nextarc = P.points[i].firstarc;
                        P.points[i].firstarc = r;
                    }
                    if(strcmp(P.points[i].data,point2->word)==0){
                        strcpy(s->word, point1->word);
                        s->nextarc = P.points[i].firstarc;
                        P.points[i].firstarc = s;
                    }
                }
                while(point3->nextarc != point2){
                    point3 = point3->nextarc;
                }
                point3->nextarc = point2->nextarc;
                point2 = point2->nextarc;
            }else{
                point2 = point2->nextarc;
            }
        }
    }
}

void sepNetListByPart(ALGraph *G, int part){
    ArcNode *point1, *point2, *point3 = NULL;
    int PointPart1,PointNextPart1 = 2;
    recordNode *r, *s;
    
    for(int x = 0; x<theLengthOfCurrentNetList; x++){
        bool jump = false;
        point1 = G->vextices[x].firstarc;
        point3 = point1;
        PointPart1 = getLorR(point1->word);
        point2 = point1->nextarc;
        for(int xx=0; xx<ALL; xx++){
            if(strcmp(point1->word, point[xx].name)==0){
                if(point[xx].part != part){
                    jump = true;
                    break;
                }
            }
        }
        if(jump==true){
            continue;
        }
        while (point2!=NULL){
            PointNextPart1 = getLorR(point2->word);
            if(PointPart1!=PointNextPart1){
                printf("cut point%s --- point%s\n",point1->word,point2->word);
                r = (recordNode *)malloc(sizeof(recordNode));
                s = (recordNode *)malloc(sizeof(recordNode));
                for(int i=0; i<ALL;i++){
                    if(strcmp(P.points[i].data,point1->word)==0){
                        strcpy(r->word, point2->word);
                        r->nextarc = P.points[i].firstarc;
                        P.points[i].firstarc = r;
                    }
                    if(strcmp(P.points[i].data,point2->word)==0){
                        strcpy(s->word, point1->word);
                        s->nextarc = P.points[i].firstarc;
                        P.points[i].firstarc = s;
                    }
                }
                while(point3->nextarc != point2){
                    point3 = point3->nextarc;
                }
                point3->nextarc = point2->nextarc;
                point2 = point2->nextarc;
            }else{
                point2 = point2->nextarc;
            }
        }
    }
}


//void doFM(ALGraph *G){
//    int theNumMaxgain = 1;
//    bool end = false;
//    while(end !=true){
//        int theCutsize = calCutsize(G);
//        theNumMaxgain = calMaxGain(G);
//        if(theNumMaxgain == -9988){
//            end = true;
//        }
//        printf("The maxGain is: %d and it is %s \n", theNumMaxgain, point[thepointNumYouMove].name);
//        //check point constrain
//        while(judgePointAfterMove(thepointNumYouMove) != 1){
//            printf("%s unsatisfy point constrain\n",  point[thepointNumYouMove].name);
//            point[thepointNumYouMove].used = 1;
//            int nowpoint = findAnotherPoint(point[thepointNumYouMove].gain);
//            if(nowpoint != -1){
//                thepointNumYouMove = nowpoint;
//            }else{
//                printf("can't find another point satisfy point constrain!\n");
//                end = true;
//                break;
//            }
//        }
//
//        //check area constrain
//        while(judgeAreaAAfterMove(thepointNumYouMove) != 1){
//            printf("%s unsatisfy area constrain\n",  point[thepointNumYouMove].name);
//            point[thepointNumYouMove].used = 1;
//            int nowpoint = findAnotherPoint(point[thepointNumYouMove].gain);
//            if(nowpoint != -1){
//                thepointNumYouMove = nowpoint;
//            }else{
//                printf("can't find another point satisfy area constrain!\n");
//                end = true;
//                break;
//            }
//        }
//        if(point[thepointNumYouMove].gain<0){
//            end = true;
//            printf("point move process end! \n");
//        }
//        if(end!=true){
//            printf("cell %s satify area constrain \n" , point[thepointNumYouMove].name);
//            movePoint(thepointNumYouMove);
//            int cs = calCutsize(G);
//            printf("Now Cutsize is %d !! \n", cs);
//        }
//    }
//    if(LEFT!=Right){
//        printf("找平衡！\n");
//        moveBackPoint(thepointNumYouMove);
//    }
//}

void resetLR(){
    for(int i = 0; i<ALL; i++){
        if(point[i].Lock==1){
            continue;
        }else{
            point[i].left_or_right = rand()%2;
            point[i].Lock = 0;
            printf("reset the L and R of point %s is %d \n", point[i].name, point[i].left_or_right);
        }
        
    }
}

void updatePart(){
    for(int i = 0; i<ALL; i++){
        int p = point[i].part;
        if(point[i].left_or_right == 0){
            point[i].part = p+p;
        }else{
            point[i].part = p+p+1;
        }
        
    }
}
void updatePart1(int part){
    for(int inin=0; inin<ALL; inin++){
        if(point[inin].part == part){
            if(point[inin].left_or_right==0){
                point[inin].part = theMaxPart + 1;
            }else{
                point[inin].part = theMaxPart + 2;
            }
        }
        
    }
}

int partNum(int part){
    int number = 0;
    for(int i = 0; i<ALL; i++){
        if(point[i].part==part){
            number++;
        }
    }
    return number;
}

void reCalLR(int part){
    LEFT = 0;
    Right = 0;
    for(int i = 0; i<ALL; i++){
        if(point[i].part == part){
            if(point[i].left_or_right == 1){
                LEFT = LEFT + 1;
            }else{
                Right = Right + 1;
            }
        }
    }
}

int maxPart = 1;
void doFMForEveryPartQuad(ALGraph *G){
    int lastMove = -10;
    int theNumMaxgain = 1;
    for(int part = 0; part<maxPart; part++){
        reCalLR(part);
        if(LEFT+Right==1){
            continue;
        }else{
            if(LEFT==0 || Right==0){
                
                for(int i=0; i<ALL; i++){
                    if(point[i].part == part){
                        if(point[i].left_or_right == 1){
                            point[i].left_or_right = 0;
                            LEFT--;
                            Right++;
                        }else{
                            point[i].left_or_right = 1;
                            LEFT++;
                            Right--;
                        }
                        //printf("all of them are in one part, move %s\n", point[i].name);
                        break;
                    }
                }
            }
            
            bool end = false;
            while(end !=true){
                theNumMaxgain = calMaxGainOfPart(G, part);
                if(theNumMaxgain == -9988){
                    end = true;
                    break;
                }
                //printf("The maxGain is: %d and it is %s \n", theNumMaxgain, point[thepointNumYouMove].name);
                
                //check point constrain
                while(judgePointAfterMove(thepointNumYouMove) != 1){
                    //printf("%s unsatisfy point constrain\n",  point[thepointNumYouMove].name);
                    point[thepointNumYouMove].used = 1;
                    int nowpoint = findAnotherPointInPart(point[thepointNumYouMove].gain, part);
                    if(nowpoint != -1){
                        thepointNumYouMove = nowpoint;
                    }else{
                        //printf("can't find another point satisfy point constrain!\n");
                        end = true;
                        break;
                    }
                }
                if(point[thepointNumYouMove].gain<0){
                    end = true;
                    //printf("point move process end! \n");
                }
                if(end!=true){
                    //printf("cell %s satify area constrain \n" , point[thepointNumYouMove].name);
                    movePoint(thepointNumYouMove);
                    lastMove = thepointNumYouMove;
                    //printf("%d, %d \n", LEFT,Right);
                    if(LEFT==0 || Right==0){
                        moveBackPoint(thepointNumYouMove);
                        //printf("have one part don't have point \n");
                    }
                }
            }
        }
        if(LEFT!=Right){
            //printf("keep balance ！\n");
            if(lastMove == -10){
                moveBackPoint(thepointNumYouMove);

            }else{
                moveBackPoint(lastMove);
            }
        }
    }
}

void doFMForEveryPartMinCut(ALGraph *G){
    int Parts = 0;
    int theNumMaxgain = 1;
    for(int part = 0; part<maxPart; part++){
        reCalLR(part);
        if(LEFT+Right==1){
            Parts++;
            continue;
        }else{
            Parts = Parts + 2;
            if(LEFT==0 || Right==0){
                for(int i=0; i<ALL; i++){
                    if(point[i].part == part){
                        if(point[i].left_or_right == 1){
                            point[i].left_or_right = 0;
                        }else{
                            point[i].left_or_right = 1;
                        }
                        break;
                    }
                }
            }
            bool end = false;
            while(end !=true){
                theNumMaxgain = calMaxGainOfPart(G, part);
                if(theNumMaxgain == -9988){
                    end = true;
                    break;
                }
                //printf("The maxGain is: %d and it is %s \n", theNumMaxgain, point[thepointNumYouMove].name);
                if(point[thepointNumYouMove].gain<0){
                    end = true;
                    //printf("point move process end! \n");
                }
                if(end!=true){
                    //printf("cell %s satify area constrain \n" , point[thepointNumYouMove].name);
                    movePoint(thepointNumYouMove);
                    if(LEFT==0 || Right==0){
                        moveBackPoint(thepointNumYouMove);
                        //printf("have one part don't have point\n");
                    }
                }
            }
        }
    }
}

void checkRecord(int nextPart){
    nextPart = nextPart+1;
    recordNode *p3 = NULL;
    
    if(nextPart<2||nextPart>5){
        for(int i=0; i<ALL; i++){
            if(point[i].part == nextPart && P.points[i].firstarc != NULL){
                p3 = P.points[i].firstarc;
                while(p3 != NULL){
                    for(int k=0; k<ALL; k++){
                        if(strcmp(point[k].name, p3->word)==0){
                            if(point[k].part == nextPart-1){
                                point[i].left_or_right = point[k].left_or_right;
                                point[i].Lock = 1;
                                printf("Lock point %s and set its LR to %d \n", point[i].name, point[i].left_or_right);
                                break;
                            }
                        }
                    }
                    p3 = p3->nextarc;
                }
            }
        }
    }else{
        for(int i=0; i<ALL; i++){
            if(point[i].part == nextPart && P.points[i].firstarc != NULL){
                p3 = P.points[i].firstarc;
                while(p3 != NULL){
                    for(int k=0; k<ALL; k++){
                        if(strcmp(point[k].name, p3->word)==0){
                            if(point[k].AorB == point[i].AorB){
                                continue;
                            }else{
                                if(point[k].AorB == 0){
                                    point[i].left_or_right = 0;
                                    point[i].Lock = 1;
                                    printf("Lock point %s and set its LR to %d \n", point[i].name, point[i].left_or_right);
                                    break;
                                }else{
                                    point[i].left_or_right = 1;
                                    point[i].Lock = 1;
                                    printf("Lock point %s and set its LR to %d \n", point[i].name, point[i].left_or_right);
                                    break;
                                }
                            }
                        }
                    }
                    p3 = p3->nextarc;
                }
            }
        }
    }
}

void doFMForEveryPartBieTerminal(ALGraph *G, int part){
    int lastMove = -10;
    int theNumMaxgain = 1;
    reCalLR(part);
    if(LEFT+Right==1){
    }else{
        if(LEFT==0 || Right==0){
            
            for(int i=0; i<ALL; i++){
                if(point[i].part == part){
                    if(point[i].Lock!=1){
                        if(point[i].left_or_right == 1){
                            point[i].left_or_right = 0;
                            LEFT--;
                            Right++;
                        }else{
                            point[i].left_or_right = 1;
                            LEFT++;
                            Right--;
                        }
                        //printf("they are all in one part so move %s\n", point[i].name);
                        break;
                    }
                    
                }
            }
        }
        bool end = false;
        while(end !=true){
            theNumMaxgain = calMaxGainOfPart(G, part);
            if(theNumMaxgain == -9988){
                end = true;
                break;
            }
            //printf("The maxGain is: %d and it is %s \n", theNumMaxgain, point[thepointNumYouMove].name);
            
            //check point constrain
            while(judgePointAfterMove(thepointNumYouMove) != 1){
                //printf("%s unsatisfy point constrain\n",  point[thepointNumYouMove].name);
                point[thepointNumYouMove].used = 1;
                int nowpoint = findAnotherPointInPart(point[thepointNumYouMove].gain, part);
                if(nowpoint != -1){
                    thepointNumYouMove = nowpoint;
                }else{
                    //printf("can't find another point satisfy point constrain!\n");
                    end = true;
                    break;
                }
            }
            if(point[thepointNumYouMove].gain<0){
                end = true;
                //printf("point move process end! \n");
            }
            if(end!=true){
                //printf("cell %s satify area constrain \n" , point[thepointNumYouMove].name);
                movePoint(thepointNumYouMove);
                lastMove = thepointNumYouMove;
                printf("%d, %d \n", LEFT,Right);
                if(LEFT==0 || Right==0){
                    moveBackPoint(thepointNumYouMove);
                    //printf("have one part don't have point\n");
                }
            }
        }
        if(LEFT!=Right){
            //printf("keep balance ！\n");
            if(lastMove == -10){
                moveBackPoint(thepointNumYouMove);

            }else{
                moveBackPoint(lastMove);
            }
        }
    }
    
}

void showPart(){
    for(int i = 0; i<ALL; i++){
        printf("%s 's part is %d \n", point[i].name,point[i].part);
    }
}
void showLR(){
    for(int i = 0; i<ALL; i++){
        printf("%s 's part is %d \n", point[i].name,point[i].left_or_right);
    }
}
void updateAorB(){
    for(int y = 0; y<ALL; y++){
        if(point[y].part == 0){
            point[y].AorB = 0;
        }else{
            point[y].AorB = 1;
        }
    }
}

void unLock(){
    for(int s = 0; s<ALL; s++){
        point[s].Lock = 0;
    }
}

int main(void)
{
    areFileRead();
    printf("we have input .are document ！ \n");
    ALGraph G;
    netFileRead(&G);
    printf("we have input .net document ！ \n");
    printf("The adjacency list structure is created \n");
    for(int i=0; i<ALL; i++){
        strcpy(P.points[i].data,point[i].name);
        P.points->firstarc=NULL;
    }
    int cut = 0;

    while(true){
        // quad placement
//        doFMForEveryPartQuad(&G);
//        updatePart();
//        sepNetList(&G);
//        resetLR();
//        maxPart = maxPart*2;
//        showPart();
//        int everyPart=0;
//        for(int i=0; i<ALL;i++){
//            if(point[i].part == 0){
//                everyPart++;
//            }
//        }
//        if(everyPart==1){
//            break;
//        }
        
        // bisection placement + terminal propagation
        if(cut<1){
            cut = cut+1;
            doFMForEveryPartQuad(&G);
            updatePart();
            updateAorB();
            theMaxPart = theMaxPart + 1;
            sepNetList(&G);
            unLock();
            resetLR();
            showPart();

        }else{
            for(int pt=0; pt<14; pt++){
                doFMForEveryPartBieTerminal(&G, pt);
                unLock();
                checkRecord(pt);
                sepNetListByPart(&G, pt);
                updatePart1(pt);
                showPart();
                theMaxPart = theMaxPart + 2;
                resetLR();
            }
        }
        int everyPart=0;
        for(int i=0; i<ALL;i++){
            if(point[i].part == 0){
                everyPart++;
            }
        }
        if(everyPart==0){
            break;
        }
   
//        mincut placement
//        resetLR();
//        doFMForEveryPartMinCut(&G);
//        updatePart();
//        sepNetList(&G);
//        maxPart = maxPart*2;
//        showPart();
//        if(isDone == true){
//            break;
//        }
        
    }
    return 0;
}


