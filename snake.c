#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>//获取系统时间，用于后面生成随机食物
#include <conio.h>//提供控制台输入输出函数
#include <windows.h>//提供文件操作的相关函数，以及光标移动

#define UP 72 //方向键：上
#define DOWN 80 //方向键：下
#define LEFT 75 //方向键：左
#define RIGHT 77 //方向键：右
//因为用非管理员模式运行代码时，如果地图太大，终端显示会出现bug
//所以要设置最大的地图长度和宽度
#define max_width 30//地图最大宽度
#define max_len 60//地图最大长度
//定义难度等级，数值是后面Sleep的时间
#define easy        200
#define medium      150
#define hard        100 
#define veryhard     80

//函数声明
void showMenu();//展示主菜单
void carryMenu(char ch);//执行主菜单
void playGame();//游戏主体（对应主菜单A选项）
void showMenu_B();//显示菜单B
void changeDifficulty(char ch);//更改难度（对应主菜单B选项）
void showMenu_C();//显示菜单C
void carryMenu_C(char ch);//排序记录（对应主菜单C选项）
void rankTime();//按时间排序
void rankScore();//按分数排序
void rankName();//按名字排序
void showRank();//展示排行榜
void changeMapsize();//修改地图大小（对应主菜单D选项）
void initializeMap();//初始化地图
void initializeSnake();//初始化蛇
void generateFood(int *foodx, int *foody);//生成食物

char *getTime();//获取当前时间
void saveFile(char* name, int score, char* time1, char* time2);//文件保存
void readFile();//文件读取
//该游戏有两个链表，所以需要两个释放的函数
void freeSnake();//释放蛇链表
void freeData();//释放数据链表
char *transDifficultyToString(int diff);//将难度转化成对应的字符串
void swapNum(int* p, int* q);//交换两个数字值
void swapStr(char* p, char* q);//交换两段字符串
int sortName(char* name1, char* name2);//根据名字进行排序

void hideCursor();//隐藏光标
void showCursor();//展示光标
void jumpCursor(int x, int y);//跳转光标

//定义位置节点
typedef struct positionNode{
    int x;
    int y;
    struct positionNode* next;
    struct positionNode* former;
}pos;
pos* snakehead = NULL;
pos* snakelast = NULL;
//初始化地图
int WIDTH = 22;
int LENGTH = 42;
char map[max_width][max_len];

//定义数据节点（排行时要读取文件数据）
typedef struct _data{
    //变量名前面加个f是为了和当前函数的变量分开
    char fname[100];//定义用户名
    int fscore;//定义分数
    int fdifficulty;//定义难度
    char ftime1[100];//起始时间
    char ftime2[100];//结束时间
    struct _data* fnext;
}data;
data* datahead = NULL;
data* datalast = NULL;

//定义主菜单（后面menu_B，menu_C为分菜单）
char menu[][100] = {
    "=====================",
    "   [A] 开始游戏  ",
    "   [B] 难度选择  ",
    "   [C] 历史记录  ",
    "   [D] 地图尺寸  ",
    "   [Q] 退出      ",
    "=====================",
};
int len = sizeof(menu) / sizeof(menu[0]);//选项个数
int option = 1;//初始化选项

//定义分菜单B
int difficulty = medium;//默认难度（对应选项二）
char menu_B[][100] = {
    "=====================",
    "   [A] 我是小蛇    ",
    "   [B] 我是蛇蛇    ",
    "   [C] 我是老蛇    ",
    "   [D] 我是老老蛇  ",
    "   [Q] 返回主菜单  ",
    "=====================",
};
int len2 = sizeof(menu_B) / sizeof(menu_B[0]);

//定义菜单C
char menu_C[][100] = {
    "=====================",
    "   [A] 按时间排序  ",
    "   [B] 按分数排序  ",
    "   [C] 按名字排序  ",
    "   [Q] 退出       ",
    "=====================",
};
int len3 = sizeof(menu_C) / sizeof(menu_C[0]);


int main(){
    //展示主菜单
    showMenu();
    return 0;
}

void showMenu(){
    //清屏，后续会不断调用这个函数,在调用后的第一时间清屏可以防止忘记在其他函数中清屏
    system("cls");
    //初始化选项为1
    option = 1;
    //打印菜单
    for (int i = 0; i < len; i++) {
        printf("%s", menu[i]);
        printf("\n");
    }
    //光标跳转到选项所指的位置，打印菜单指引 "<<" 和 ">>""
    jumpCursor(0, option);
    printf("<<");
    jumpCursor(strlen(menu[option]), option);
    printf(">>");
    jumpCursor(0, len);

    while(1){
        //检测玩家有没有输入
        if(kbhit()){
            hideCursor();//如果有就先隐藏起光标
            //如果用户输入UP键
            if(GetAsyncKeyState(VK_UP)) {
                getch();getch();//可以理解为清键盘缓冲区
                if(option != 1){
                    //先删除当前的<< >>
                    jumpCursor(0, option);
                    printf("  ");
                    jumpCursor(strlen(menu[option]), option);
                    printf("  ");
                    option--;
                    //再移动到上一个选项并打印<< >>
                    jumpCursor(0, option);
                    printf("<<");
                    jumpCursor(strlen(menu[option]), option);
                    printf(">>");
                }
                //当玩家在第一个选项处按下UP键时，菜单指引会跳到最后一个，让菜单显得更丝滑一点
                else{
                    //先删除当前的<< >>
                    jumpCursor(0, option);
                    printf("  ");
                    jumpCursor(strlen(menu[option]), option);
                    printf("  ");
                    option = len - 2;
                    //再移动到最后一个选项并打印<< >>
                    jumpCursor(0, option);
                    printf("<<");
                    jumpCursor(strlen(menu[option]), option);
                    printf(">>");
                }
            } 
            //如果用户输入DOWN键，基本实现原理跟上一个相同
            else if(GetAsyncKeyState(VK_DOWN)){
                getch();getch();
                if(option != len - 2){
                    jumpCursor(0, option);
                    printf("  ");
                    jumpCursor(strlen(menu[option]), option);
                    printf("  ");
                    option++;
                    jumpCursor(0, option);
                    printf("<<");
                    jumpCursor(strlen(menu[option]), option);
                    printf(">>");
                }
                else{
                    jumpCursor(0, option);
                    printf("  ");
                    jumpCursor(strlen(menu[option]), option);
                    printf("  ");
                    option = 1;
                    jumpCursor(0, option);
                    printf("<<");
                    jumpCursor(strlen(menu[option]), option);
                    printf(">>");
                }
            }
            else{
                //执行菜单
                char ch = getch();//getch()可以用于获取输入，同时隐藏用户输入的内容
                //如果是回车，直接执行当前选项内容
                if (ch == '\r') {
                    //如果在option在最后一个选项，按下回车则直接退出
                    if(option == len - 2){
                        freeSnake();//记得先释放链表
                        exit(0);//可以直接结束掉整个游戏
                    }
                    //如果不是的话把选项换做大写英文字母
                    ch = 'A' + option - 1;
                    system("cls");
                    carryMenu(ch);//调用carryMenu函数执行对应选项
                }
                else {
                    //如果用户不是输入回车，那么根据用户输入内容执行相应选项
                    if(ch >= 'a' && ch <= 'z') ch = ch - 'a' + 'A';//转换成大写字母，与后面统一
                    if(ch == 'Q'){
                        freeSnake();
                        exit(0);
                    }
                    carryMenu(ch);
                }
            }
            //光标跳回原位
            showCursor();
            jumpCursor(0, len);
        }
    }
}

void carryMenu(char ch){
    //需要ch作为参数，代表用户选择了哪个选项
    switch (ch) {
        case 'A': playGame();               break;
        case 'B': showMenu_B();             break;
        case 'C': showMenu_C();             break;
        case 'D': changeMapsize();          break;
        //Q选项在前面已经特判过了，这里不需要再增加
        default : 
            printf("你这选的啥啊\n");
            Sleep(1200);
            system("cls");
            showMenu();
    }
}

void playGame(){
    system("cls");
    //以时间作为随机函数的种子
    srand(time(NULL));
    //输入用户名
    char name[100];
    printf("请输入用户名: ");
    scanf("%s", name);
    system("cls");
    //隐藏光标
    hideCursor();
    //初始化地图
    initializeMap();
    //初始化蛇
    initializeSnake();
    //初始化食物
    int foodx = 0, foody = 0;
    generateFood(&foodx, &foody);
    //初始化分数
    int score = 0;
    //获取当前时间（起始时间）
    char time1[100];
    strcpy(time1, getTime());

    //在非游戏区打印一些游戏信息（例如用户名，当前分数，难度）
    //具体位置可以随意调整
    jumpCursor(0, WIDTH);
    printf("玩家 %s 当前分数为： %d", name, score);
    jumpCursor(LENGTH, 0);
    printf(" 难度： %s", transDifficultyToString(difficulty));
    jumpCursor(LENGTH, 1);
    printf(" 按j可以进行加速");
    //初始化方向，往右（这里d是指wasd这个方向键的d）
    char direct = 'd';
    char tmp_direct = 'd';//临时方向，下面会用上

    //游戏逻辑主体
    while(1){
        //按键改变蛇头方向
        if(_kbhit()){
            //先不急着改变现在蛇头的方向，用临时方向存放下
            tmp_direct = getch();
            //将方向键转化为字母键，方便后续处理
            if(tmp_direct == UP) tmp_direct = 'w';
            else if(tmp_direct == LEFT) tmp_direct = 'a';
            else if(tmp_direct == DOWN) tmp_direct = 's';
            else if(tmp_direct == RIGHT) tmp_direct = 'd';
            //如果按键不互补，将临时方向赋给现在的方向（互补一个例子就是蛇头朝上时，玩家按下w或s键将不会有响应）
            if((tmp_direct == 'w' || tmp_direct == 's') && (direct == 'a' || direct == 'd')) 
                direct = tmp_direct;
            else if((tmp_direct == 'a' || tmp_direct == 'd') && (direct == 'w' || direct == 's')) 
                direct = tmp_direct;
        }
        
        //更新蛇的位置
        //先把蛇尾位置保存一下
        int tmp_x = snakelast->x;
        int tmp_y = snakelast->y;
        pos* p = snakelast;//临时节点p
        //把前一个节点的坐标赋给后一个节点即可
        while(p->former){
            p->x = p->former->x;
            p->y = p->former->y;
            p = p->former;
        }
        //这一段是用单向链表，效果和双向链表完全相同 
        // while (p != snakehead) {
        //     pos* q = snakehead;
        //     while (q->next != p) {
        //         q = q->next;
        //     }
        //     p->x = q->x; p->y = q->y;
        //     p = q;
        // }

        //根据方向对蛇头位置进行移动
        if(direct == 'w') snakehead->y--;
        else if(direct == 'a') snakehead->x--;
        else if(direct == 's') snakehead->y++;
        else if(direct == 'd') snakehead->x++;

        //撞到墙或咬到蛇身则游戏结束
        if(map[snakehead->y][snakehead->x] == '#' || map[snakehead->y][snakehead->x] == 'o') {
            jumpCursor(0, WIDTH + 1);
            printf("Game Over!\n");
            break;
        }

        //打印蛇（先删蛇尾，再从蛇头开始打印蛇身）
        jumpCursor(tmp_x, tmp_y);//先将蛇尾删除
        printf(" ");
        map[tmp_y][tmp_x] = ' ';
        jumpCursor(snakehead->x, snakehead->y);//再打印蛇头
        printf("O");
        map[snakehead->y][snakehead->x] = 'O';
        pos* q = snakehead->next;
        while(q){
            jumpCursor(q->x, q->y);//打印蛇身
            printf("o");
            map[q->y][q->x] = 'o';
            q = q->next;
        }

        //打印结束后要让程序睡眠一下，不然程序执行太快，蛇的运动会太快
        //睡眠时间根据玩家选择的难度而定，难度越高，睡眠时间越短，蛇的运动会越快
        //如果玩家按下了j键，睡眠时间可以短一点，这样蛇的运动速度就会变快
        if(tmp_direct == 'j'){
            Sleep(10);
            tmp_direct = ' ';
        }else{
            Sleep(difficulty);
        }
        
        //吃到食物(表现为食物的位置被替换成了蛇头)
        if(map[foody][foodx] == 'O'){
            //跳转到蛇尾消失处，打印新蛇尾
            jumpCursor(tmp_x, tmp_y);
            printf("o");
            map[tmp_y][tmp_x] = 'o';
            //将新蛇尾加入链表
            pos* tmp_node = (pos*)malloc(sizeof(pos));
            tmp_node->x = tmp_x;
            tmp_node->y = tmp_y;
            tmp_node->former = snakelast;
            tmp_node->next = NULL;
            snakelast->next = tmp_node;
            snakelast = snakelast->next;
            //产生新食物
            generateFood(&foodx, &foody);
            //分数增加
            score += 10;
            //更新分数显示
            jumpCursor(0, WIDTH);
            printf("玩家 %s 当前分数为： %d", name, score);
        }
    }

    //获取结束时间
    char time2[100];
    strcpy(time2, getTime());
    
    //保存名字，分数，时间和难度
    saveFile(name, score, time1, time2);

    //清理字符串
    memset(time1, 0, sizeof(time1));
    memset(time2, 0, sizeof(time2));
    //释放链表
    freeSnake();

    fflush(stdin);//清理键盘缓冲区
    char again[100];
    //询问是否再来一把
    while(1){
        printf("是否再来一把? yes/no:\n");
        gets(again);
        if(strcmp(again, "yes") == 0) {
            system("cls");
            playGame();
        }
        else if(strcmp(again, "no") == 0){
            printf("游戏结束\n");
            system("cls");
            showMenu();
            break;
        }
        else{
            printf("无效输入qwq\n");
        }
    }
}

void showMenu_B(){
    system("cls");
    //打印B选项的菜单
    for(int i = 0; i < len2; i++){
        printf("%s\n", menu_B[i]);
    }
    option = 2;//默认难度是第二个
    jumpCursor(0, option);
    printf("<<");
    jumpCursor(strlen(menu_B[option]), option);
    printf(">>");
    jumpCursor(0, len2);

    while(1){
        if(kbhit()){
            hideCursor();
            if(GetAsyncKeyState(VK_UP)) {
                getch();getch();
                if(option != 1){
                    //先删除当前的<< >>
                    jumpCursor(0, option);
                    printf("  ");
                    jumpCursor(strlen(menu_B[option]), option);
                    printf("  ");
                    option--;
                    //再移动到上一个选项并打印<< >>
                    jumpCursor(0, option);
                    printf("<<");
                    jumpCursor(strlen(menu_B[option]), option);
                    printf(">>");
                }
                else{
                    //先删除当前的<< >>
                    jumpCursor(0, option);
                    printf("  ");
                    jumpCursor(strlen(menu_B[option]), option);
                    printf("  ");
                    option = len2 - 2;
                    //再移动到最后一个选项并打印<< >>
                    jumpCursor(0, option);
                    printf("<<");
                    jumpCursor(strlen(menu_B[option]), option);
                    printf(">>");
                }
            } 
            else if(GetAsyncKeyState(VK_DOWN)){
                getch();getch();
                if(option != len2 - 2){
                    jumpCursor(0, option);
                    printf("  ");
                    jumpCursor(strlen(menu_B[option]), option);
                    printf("  ");
                    option++;
                    jumpCursor(0, option);
                    printf("<<");
                    jumpCursor(strlen(menu_B[option]), option);
                    printf(">>");
                }
                else{
                    //先删除当前的<< >>
                    jumpCursor(0, option);
                    printf("  ");
                    jumpCursor(strlen(menu_B[option]), option);
                    printf("  ");
                    option = 1;
                    //再移动到第一个选项并打印<< >>
                    jumpCursor(0, option);
                    printf("<<");
                    jumpCursor(strlen(menu_B[option]), option);
                    printf(">>");
                }
            }
            else{
                char ch = getch();
                if(ch == '\r'){
                    if(option == len2 - 2) showMenu();
                    ch = 'A' + option - 1;
                    system("cls");
                    changeDifficulty(ch);
                }
                else {
                    if(ch >= 'a' && ch <= 'z') ch = ch - 'a' + 'A';
                    system("cls");
                    if(ch == 'Q') showMenu();
                    else changeDifficulty(ch);
                }
            }
        }
    }
}

void changeDifficulty(char ch){
    switch(ch) {
        case 'A': difficulty = easy;        break;
        case 'B': difficulty = medium;      break;
        case 'C': difficulty = hard;        break;
        case 'D': difficulty = veryhard;    break;
        default : 
            printf("你这选的啥啊\n");
            Sleep(1200);
            system("cls");
            showMenu_B();
    }
    printf("难度更改完毕！\n");
    Sleep(1200);
    system("cls");
    showMenu();
}

void showMenu_C(){
    system("cls");
    //打印C选项的菜单
    for(int i = 0; i < len3; i++){
        printf("%s\n", menu_C[i]);
    }
    option = 1;//默认难度是第一个
    jumpCursor(0, option);
    printf("<<");
    jumpCursor(strlen(menu_C[option]), option);
    printf(">>");
    jumpCursor(0, len3);

    while(1){
        if(kbhit()){
            hideCursor();
            if(GetAsyncKeyState(VK_UP)) {
                getch();getch();
                if(option != 1){
                    //先删除当前的<< >>
                    jumpCursor(0, option);
                    printf("  ");
                    jumpCursor(strlen(menu_C[option]), option);
                    printf("  ");
                    option--;
                    //再移动到上一个选项并打印<< >>
                    jumpCursor(0, option);
                    printf("<<");
                    jumpCursor(strlen(menu_C[option]), option);
                    printf(">>");
                }
                else{
                    //先删除当前的<< >>
                    jumpCursor(0, option);
                    printf("  ");
                    jumpCursor(strlen(menu_C[option]), option);
                    printf("  ");
                    option = len3 - 2;
                    //再移动到最后一个选项并打印<< >>
                    jumpCursor(0, option);
                    printf("<<");
                    jumpCursor(strlen(menu_C[option]), option);
                    printf(">>");
                }
            } 
            else if(GetAsyncKeyState(VK_DOWN)){
                getch();getch();
                if(option != len3 - 2){
                    jumpCursor(0, option);
                    printf("  ");
                    jumpCursor(strlen(menu_C[option]), option);
                    printf("  ");
                    option++;
                    jumpCursor(0, option);
                    printf("<<");
                    jumpCursor(strlen(menu_C[option]), option);
                    printf(">>");
                }
                else{
                    //先删除当前的<< >>
                    jumpCursor(0, option);
                    printf("  ");
                    jumpCursor(strlen(menu_C[option]), option);
                    printf("  ");
                    option = 1;
                    //再移动到第一个选项并打印<< >>
                    jumpCursor(0, option);
                    printf("<<");
                    jumpCursor(strlen(menu_C[option]), option);
                    printf(">>");
                }
            }
            else{
                char ch = getch();
                if(ch == '\r') {
                    if(option == len3 - 2) showMenu();
                    ch = 'A' + option - 1;
                    system("cls");
                    carryMenu_C(ch);
                }
                else {
                    if(ch >= 'a' && ch <= 'z') ch = ch - 'a' + 'A';
                    system("cls");
                    if(ch == 'Q') showMenu();
                    else carryMenu_C(ch);
                }
            }
        }
    }
}

void carryMenu_C(char ch){
    readFile();
    switch(ch) {
        case 'A': rankTime();   break;                   
        case 'B': rankScore();  break;  
        case 'C': rankName();   break;    
        default : 
            printf("你这选的啥啊\n");
            Sleep(1200);
            system("cls");
            showMenu_C();
    }
    printf("以上是排行榜\n");
    system("pause");
    system("cls");
    //读取完数据就释放掉，正常玩游戏不需要调用这些数据
    freeData();
    showMenu_C();
}

void rankTime(){
    //将单向链表的顺序就地逆置
    //每次让后一个节点的fnext指向前一个节点
    data* p = datalast;
    while (p != datahead) {
        data* q = datahead;
        while (q->fnext != p) {
            q = q->fnext;
        }
        p->fnext = q;
        p = q;
    }
    p->fnext = NULL;
    datahead = datalast;
    datalast = p;
    showRank();
}

void rankScore(){
    int k = 1;//接下来是冒泡排序
    while(k == 1){
        k = 0;
        data* p = datahead;
        while(p->fnext){
            //先根据难度大小进行排序
            if(p->fdifficulty > p->fnext->fdifficulty){
                k = 1;
                swapNum(&p->fscore, &p->fnext->fscore);
                swapNum(&p->fdifficulty, &p->fnext->fdifficulty);
                swapStr(p->fname, p->fnext->fname);
                swapStr(p->ftime1, p->fnext->ftime1);
                swapStr(p->ftime2, p->fnext->ftime2);
            }
            p = p->fnext;
        }
    }
    k = 1;
    while(k == 1){
        k = 0;
        data* p = datahead;
        while(p->fnext){
            //再根据分数大小进行排序(难度相等才能进行排序)
            if(p->fdifficulty == p->fnext->fdifficulty && p->fscore < p->fnext->fscore){
                k = 1;
                swapNum(&p->fscore, &p->fnext->fscore);
                swapNum(&p->fdifficulty, &p->fnext->fdifficulty);
                swapStr(p->fname, p->fnext->fname);
                swapStr(p->ftime1, p->fnext->ftime1);
                swapStr(p->ftime2, p->fnext->ftime2);
            }
            p = p->fnext;
        }
    }
    showRank();
}

void rankName(){
    int k = 1;//接下来是冒泡排序
    while(k == 1){
        k = 0;
        data* p = datahead;
        while(p->fnext){
            //根据名字首字母,长短进行排序
            //名字排序规则在sortName函数中体现
            if(sortName(p->fname, p->fnext->fname)){
                k = 1;
                swapNum(&p->fscore, &p->fnext->fscore);
                swapNum(&p->fdifficulty, &p->fnext->fdifficulty);
                swapStr(p->fname, p->fnext->fname);
                swapStr(p->ftime1, p->fnext->ftime1);
                swapStr(p->ftime2, p->fnext->ftime2);
                //printf("check1\n");
            }
            p = p->fnext;
        }
    }
    showRank();
}

void showRank(){
    while(datahead){
        printf("用户名：    %s\n",  datahead->fname);
        printf("得分：      %d\n",  datahead->fscore);
        //难度是数值，要把它转化为字符串以便玩家能理解
        printf("难度：      %s\n",  transDifficultyToString(datahead->fdifficulty));
        printf("开始时间：  %s\n",  datahead->ftime1);
        printf("结束时间：  %s\n",  datahead->ftime2);
        printf("\n");
        datahead = datahead->fnext;
    }
}

void changeMapsize(){
    //用临时长度和宽度存一下，判断是否超过限制
    int tmp_length;
    int tmp_width;
    //要有最小值的原因是因为后面要设置障碍物，地图太小障碍物不好设置
    printf("请输入地图的长度：(需小于%d且大于20)\n", max_len);
    scanf("%d", &tmp_length);
    printf("请输入地图的宽度: (需小于%d且大于20)\n", max_width);
    scanf("%d", &tmp_width);

    //检测是否越界
    if(tmp_length >= max_len || tmp_width >= max_width 
    || tmp_length <= 20 || tmp_width <= 20){
        system("cls");
        printf("地图尺寸改变失败！\n");
        Sleep(1200);
        showMenu();
    }
    else {
        LENGTH = tmp_length;
        WIDTH = tmp_width;
        system("cls");
        printf("地图尺寸改变成功！\n");
        printf("长度被更改为%d, 宽度被更改为%d", LENGTH, WIDTH);
        Sleep(1200);
        showMenu();
    }
}

void initializeMap(){
    //打印地图四周的墙
    memset(map, ' ', sizeof(map));
    for(int j = 0; j < LENGTH; j++) map[0][j] = '#';
    for(int i = 0; i < WIDTH; i++){
        map[i][0] = '#';
        map[i][LENGTH - 1] = '#';
    }
    for(int j = 0; j < LENGTH; j++) map[WIDTH - 1][j] = '#';

    //如果难度为老老蛇，引入障碍物概念
    //下面是我做的一个地图，大致样子如下
    /*
    #############################
    #                           #
    #    #####        #####     #
    #    #####        #####     #
    #                           #
    #                           #
    #    #####        #####     #
    #    #####        #####     #
    #                           #
    #############################
    */
    //实现原理就是先根据地图大小确定障碍物长度和宽度
    //再在离墙大概4个空格的地方打印障碍物
    if(difficulty == veryhard){
        int olen = LENGTH / 2 / 2; //olen表示障碍物长度
        int owid = WIDTH / 2 / 2;  //owid表示障碍物宽度
        for(int i = 4; i < 4 + owid; i++){
            for(int j = 4; j < 4 + olen; j++){
                map[i][j] = '#';
            }
        }
        for(int i = 4; i < 4 + owid; i++){
            for(int j = LENGTH - 4 - olen; j < LENGTH - 4; j++) {
                map[i][j] = '#';
            }
        }
        for(int i = WIDTH - 4 - owid; i < WIDTH - 4; i++){
            for(int j = 4; j < 4 + olen; j++){
                map[i][j] = '#';
            }
        }
        for(int i = WIDTH - 4 - owid; i < WIDTH - 4; i++){
            for(int j = LENGTH - 4 - olen; j < LENGTH - 4; j++){
                map[i][j] = '#';
            }
        }
    }

    for(int i = 0; i < WIDTH; i++){
        for(int j = 0; j < LENGTH; j++){
            printf("%c", map[i][j]);
        }
        printf("\n");
    }

}

void initializeSnake(){
    //初始化蛇为 ooO
    //蛇头
    snakehead = (pos*)malloc(sizeof(pos));
    snakehead->x = LENGTH / 2;
    snakehead->y = WIDTH / 2;
    snakehead->next = NULL;
    snakehead->former = NULL;
    //第一节蛇身
    pos* body = (pos*)malloc(sizeof(pos));
    body->x = snakehead->x - 1; 
    body->y = snakehead->y;
    body->former = snakehead;
    body->next = NULL;
    snakehead->next = body;
    //第二节蛇身（也即是最后一节）
    snakelast = (pos*)malloc(sizeof(pos));
    snakelast->x = snakehead->x - 2; 
    snakelast->y = snakehead->y;
    snakelast->former = body;
    snakelast->next = NULL;
    body->next = snakelast;
}

void generateFood(int *foodx, int *foody){
    //在空格地方生成食物
    while(map[*foody][*foodx] != ' '){
        *foodx = rand() % LENGTH;
        *foody = rand() % WIDTH;
    }
    jumpCursor(*foodx, *foody);
    printf("*");
    map[*foody][*foodx] = '*';
}

char *getTime(){
    //获取当前时间并返回时间字符串
    time_t now;
    struct tm *local_time;
    static char time_str[100]; //确保这个字符串足够大以存储格式化后的时间
    memset(time_str, 0, sizeof(time_str));

    time(&now);
    local_time = localtime(&now);

    //使用 snprintf 来避免缓冲区溢出
    snprintf(time_str, sizeof(time_str), "%d-%02d-%02d %02d:%02d:%02d",
             local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
             local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

    return time_str;
}

void saveFile(char* name, int score, char* time1, char* time2){
    FILE* fptr = fopen("snakefile.txt", "a");//打开文件追加模式
    if (!fptr) {
        printf("读取文件时发生错误\n");
        return;
    }
    fprintf(fptr, "%s\n", name);
    fprintf(fptr, "%d\n", score);
    fprintf(fptr, "%d\n", difficulty);
    fprintf(fptr, "%s\n", time1);
    fprintf(fptr, "%s\n", time2);
    fclose(fptr);
}

void readFile(){ 
    FILE* fptr = fopen("snakefile.txt", "r");
    if (!fptr) {
        printf("读取文件时发生错误\n");
        return;
    }
    //定义名字，分数等变量，用于读取文件中的数据
    char fname[100];
    int  fscore;
    int  fdifficulty;
    char ftime1[100];
    char ftime2[100];

    //当读到文件末尾时fgets函数会返回NULL
    while( fgets(fname, sizeof(fname), fptr) != NULL ){
        //fgets函数会读取换行符，要将换行符位置改成'\0'
        fname[strlen(fname) - 1] = '\0';
        //fgets函数读取第一行的名字，fscanf函数读取第二行的成绩
        //以此类推
        fscanf(fptr, "%d\n", &fscore);
        fscanf(fptr, "%d\n", &fdifficulty);
        fgets(ftime1, sizeof(ftime1), fptr); ftime1[strlen(ftime1) - 1] = '\0';
        fgets(ftime2, sizeof(ftime2), fptr); ftime2[strlen(ftime2) - 1] = '\0';

        //定义临时节点tmp_node，malloc空间之后把读到的数据都传上去
        //至于为什么不直接给last->next分配空间并读上去，是因为用临时节点存放比较直观
        data* tmp_node = (data*)malloc(sizeof(data));
        strcpy(tmp_node->fname, fname);
        tmp_node->fscore = fscore;
        tmp_node->fdifficulty = fdifficulty;
        strcpy(tmp_node->ftime1, ftime1);
        strcpy(tmp_node->ftime2, ftime2);
        tmp_node->fnext = NULL;

        if(datahead){
            datalast->fnext = tmp_node;
            datalast = tmp_node;
        }
        else{
            datahead = tmp_node;
            datalast = datahead;
        }
        //清空数组，继续接受下一次输入
        memset(fname, 0, sizeof(fname));
        memset(ftime1, 0, sizeof(ftime1));
        memset(ftime2, 0, sizeof(ftime2));
    }
    //关闭文件
    fclose(fptr);   
}

void freeSnake(){
    while(snakehead){
        pos* p = snakehead;
        snakehead = snakehead->next;
        free(p);
    }
}

void freeData(){
    while(datahead){
        data* p = datahead;
        datahead = datahead->fnext;
        free(p);
    }
}

char *transDifficultyToString(int diff){
    //把难度数值转换成对应的字符串
    //例如当difficulty = 150时，返回字符串“蛇蛇”
    //定义成静态全局变量是因为离开该函数时这些字符串仍要存在
    static char d1[10] = "小蛇", d2[10] = "蛇蛇", d3[10] = "老蛇", d4[10] = "老老蛇";
    switch (diff){
        case easy:      return d1;
        case medium:    return d2;
        case hard:      return d3;
        case veryhard:  return d4;
    }
}

void swapNum(int* p, int* q){
    int t = *p;
    *p = *q;
    *q = t;
}

void swapStr(char* p, char* q){
    char t[100]; memset(t, 0, sizeof(t));
    strcpy(t, p); memset(p, 0, sizeof(p));
    strcpy(p, q); memset(q, 0, sizeof(q));
    strcpy(q, t);
}

int sortName(char* name1, char* name2){
    //名字相同，不用排序
    if(strcmp(name1, name2) == 0) return 0;

    //排序规则：先是名字首字母小的在前，后是名字短的在前
    //例如：andon > andong > maoshun
    //先比较首字母大小 
    int min = (strlen(name1) <= strlen(name2)) ? strlen(name1) : strlen(name2);
    for(int i = 0; i < min; i++){
        if(name1[i] < name2[i]){
            return 0;
        }
        else if(name1[i] > name2[i]){
            return 1;
        }
    }
    //首字母比较不了了，比较名字长短
    if(strlen(name1) == min) return 0;
    else return 1;
}

void hideCursor(){
        HANDLE hConsoleOutput;
        CONSOLE_CURSOR_INFO cursorInfo;
        // 获取当前控制台输出的句柄
        hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        // 获取当前光标信息
        GetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
        // 设置光标的可见性，第二个参数为0时隐藏光标
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
}

void showCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = TRUE; // 设置光标可见
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void jumpCursor(int x, int y){
	COORD pos; //定义光标位置的结构体变量
	pos.X = x; //横坐标
	pos.Y = y; //纵坐标
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); //获取控制台句柄
	SetConsoleCursorPosition(handle, pos); //设置光标位置
}
