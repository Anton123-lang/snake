#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>//��ȡϵͳʱ�䣬���ں����������ʳ��
#include <conio.h>//�ṩ����̨�����������
#include <windows.h>//�ṩ�ļ���������غ������Լ�����ƶ�

#define UP 72 //���������
#define DOWN 80 //���������
#define LEFT 75 //���������
#define RIGHT 77 //���������
//��Ϊ�÷ǹ���Աģʽ���д���ʱ�������ͼ̫���ն���ʾ�����bug
//����Ҫ�������ĵ�ͼ���ȺͿ��
#define max_width 30//��ͼ�����
#define max_len 60//��ͼ��󳤶�
//�����Ѷȵȼ�����ֵ�Ǻ���Sleep��ʱ��
#define easy        200
#define medium      150
#define hard        100 
#define veryhard     80

//��������
void showMenu();//չʾ���˵�
void carryMenu(char ch);//ִ�����˵�
void playGame();//��Ϸ���壨��Ӧ���˵�Aѡ�
void showMenu_B();//��ʾ�˵�B
void changeDifficulty(char ch);//�����Ѷȣ���Ӧ���˵�Bѡ�
void showMenu_C();//��ʾ�˵�C
void carryMenu_C(char ch);//�����¼����Ӧ���˵�Cѡ�
void rankTime();//��ʱ������
void rankScore();//����������
void rankName();//����������
void showRank();//չʾ���а�
void changeMapsize();//�޸ĵ�ͼ��С����Ӧ���˵�Dѡ�
void initializeMap();//��ʼ����ͼ
void initializeSnake();//��ʼ����
void generateFood(int *foodx, int *foody);//����ʳ��

char *getTime();//��ȡ��ǰʱ��
void saveFile(char* name, int score, char* time1, char* time2);//�ļ�����
void readFile();//�ļ���ȡ
//����Ϸ����������������Ҫ�����ͷŵĺ���
void freeSnake();//�ͷ�������
void freeData();//�ͷ���������
char *transDifficultyToString(int diff);//���Ѷ�ת���ɶ�Ӧ���ַ���
void swapNum(int* p, int* q);//������������ֵ
void swapStr(char* p, char* q);//���������ַ���
int sortName(char* name1, char* name2);//�������ֽ�������

void hideCursor();//���ع��
void showCursor();//չʾ���
void jumpCursor(int x, int y);//��ת���

//����λ�ýڵ�
typedef struct positionNode{
    int x;
    int y;
    struct positionNode* next;
    struct positionNode* former;
}pos;
pos* snakehead = NULL;
pos* snakelast = NULL;
//��ʼ����ͼ
int WIDTH = 22;
int LENGTH = 42;
char map[max_width][max_len];

//�������ݽڵ㣨����ʱҪ��ȡ�ļ����ݣ�
typedef struct _data{
    //������ǰ��Ӹ�f��Ϊ�˺͵�ǰ�����ı����ֿ�
    char fname[100];//�����û���
    int fscore;//�������
    int fdifficulty;//�����Ѷ�
    char ftime1[100];//��ʼʱ��
    char ftime2[100];//����ʱ��
    struct _data* fnext;
}data;
data* datahead = NULL;
data* datalast = NULL;

//�������˵�������menu_B��menu_CΪ�ֲ˵���
char menu[][100] = {
    "=====================",
    "   [A] ��ʼ��Ϸ  ",
    "   [B] �Ѷ�ѡ��  ",
    "   [C] ��ʷ��¼  ",
    "   [D] ��ͼ�ߴ�  ",
    "   [Q] �˳�      ",
    "=====================",
};
int len = sizeof(menu) / sizeof(menu[0]);//ѡ�����
int option = 1;//��ʼ��ѡ��

//����ֲ˵�B
int difficulty = medium;//Ĭ���Ѷȣ���Ӧѡ�����
char menu_B[][100] = {
    "=====================",
    "   [A] ����С��    ",
    "   [B] ��������    ",
    "   [C] ��������    ",
    "   [D] ����������  ",
    "   [Q] �������˵�  ",
    "=====================",
};
int len2 = sizeof(menu_B) / sizeof(menu_B[0]);

//����˵�C
char menu_C[][100] = {
    "=====================",
    "   [A] ��ʱ������  ",
    "   [B] ����������  ",
    "   [C] ����������  ",
    "   [Q] �˳�       ",
    "=====================",
};
int len3 = sizeof(menu_C) / sizeof(menu_C[0]);


int main(){
    //չʾ���˵�
    showMenu();
    return 0;
}

void showMenu(){
    //�����������᲻�ϵ����������,�ڵ��ú�ĵ�һʱ���������Է�ֹ��������������������
    system("cls");
    //��ʼ��ѡ��Ϊ1
    option = 1;
    //��ӡ�˵�
    for (int i = 0; i < len; i++) {
        printf("%s", menu[i]);
        printf("\n");
    }
    //�����ת��ѡ����ָ��λ�ã���ӡ�˵�ָ�� "<<" �� ">>""
    jumpCursor(0, option);
    printf("<<");
    jumpCursor(strlen(menu[option]), option);
    printf(">>");
    jumpCursor(0, len);

    while(1){
        //��������û������
        if(kbhit()){
            hideCursor();//����о�����������
            //����û�����UP��
            if(GetAsyncKeyState(VK_UP)) {
                getch();getch();//�������Ϊ����̻�����
                if(option != 1){
                    //��ɾ����ǰ��<< >>
                    jumpCursor(0, option);
                    printf("  ");
                    jumpCursor(strlen(menu[option]), option);
                    printf("  ");
                    option--;
                    //���ƶ�����һ��ѡ���ӡ<< >>
                    jumpCursor(0, option);
                    printf("<<");
                    jumpCursor(strlen(menu[option]), option);
                    printf(">>");
                }
                //������ڵ�һ��ѡ�����UP��ʱ���˵�ָ�����������һ�����ò˵��Եø�˿��һ��
                else{
                    //��ɾ����ǰ��<< >>
                    jumpCursor(0, option);
                    printf("  ");
                    jumpCursor(strlen(menu[option]), option);
                    printf("  ");
                    option = len - 2;
                    //���ƶ������һ��ѡ���ӡ<< >>
                    jumpCursor(0, option);
                    printf("<<");
                    jumpCursor(strlen(menu[option]), option);
                    printf(">>");
                }
            } 
            //����û�����DOWN��������ʵ��ԭ�����һ����ͬ
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
                //ִ�в˵�
                char ch = getch();//getch()�������ڻ�ȡ���룬ͬʱ�����û����������
                //����ǻس���ֱ��ִ�е�ǰѡ������
                if (ch == '\r') {
                    //�����option�����һ��ѡ����»س���ֱ���˳�
                    if(option == len - 2){
                        freeSnake();//�ǵ����ͷ�����
                        exit(0);//����ֱ�ӽ�����������Ϸ
                    }
                    //������ǵĻ���ѡ�����дӢ����ĸ
                    ch = 'A' + option - 1;
                    system("cls");
                    carryMenu(ch);//����carryMenu����ִ�ж�Ӧѡ��
                }
                else {
                    //����û���������س�����ô�����û���������ִ����Ӧѡ��
                    if(ch >= 'a' && ch <= 'z') ch = ch - 'a' + 'A';//ת���ɴ�д��ĸ�������ͳһ
                    if(ch == 'Q'){
                        freeSnake();
                        exit(0);
                    }
                    carryMenu(ch);
                }
            }
            //�������ԭλ
            showCursor();
            jumpCursor(0, len);
        }
    }
}

void carryMenu(char ch){
    //��Ҫch��Ϊ�����������û�ѡ�����ĸ�ѡ��
    switch (ch) {
        case 'A': playGame();               break;
        case 'B': showMenu_B();             break;
        case 'C': showMenu_C();             break;
        case 'D': changeMapsize();          break;
        //Qѡ����ǰ���Ѿ����й��ˣ����ﲻ��Ҫ������
        default : 
            printf("����ѡ��ɶ��\n");
            Sleep(1200);
            system("cls");
            showMenu();
    }
}

void playGame(){
    system("cls");
    //��ʱ����Ϊ�������������
    srand(time(NULL));
    //�����û���
    char name[100];
    printf("�������û���: ");
    scanf("%s", name);
    system("cls");
    //���ع��
    hideCursor();
    //��ʼ����ͼ
    initializeMap();
    //��ʼ����
    initializeSnake();
    //��ʼ��ʳ��
    int foodx = 0, foody = 0;
    generateFood(&foodx, &foody);
    //��ʼ������
    int score = 0;
    //��ȡ��ǰʱ�䣨��ʼʱ�䣩
    char time1[100];
    strcpy(time1, getTime());

    //�ڷ���Ϸ����ӡһЩ��Ϸ��Ϣ�������û�������ǰ�������Ѷȣ�
    //����λ�ÿ����������
    jumpCursor(0, WIDTH);
    printf("��� %s ��ǰ����Ϊ�� %d", name, score);
    jumpCursor(LENGTH, 0);
    printf(" �Ѷȣ� %s", transDifficultyToString(difficulty));
    jumpCursor(LENGTH, 1);
    printf(" ��j���Խ��м���");
    //��ʼ���������ң�����d��ָwasd����������d��
    char direct = 'd';
    char tmp_direct = 'd';//��ʱ�������������

    //��Ϸ�߼�����
    while(1){
        //�����ı���ͷ����
        if(_kbhit()){
            //�Ȳ����Ÿı�������ͷ�ķ�������ʱ��������
            tmp_direct = getch();
            //�������ת��Ϊ��ĸ���������������
            if(tmp_direct == UP) tmp_direct = 'w';
            else if(tmp_direct == LEFT) tmp_direct = 'a';
            else if(tmp_direct == DOWN) tmp_direct = 's';
            else if(tmp_direct == RIGHT) tmp_direct = 'd';
            //�������������������ʱ���򸳸����ڵķ��򣨻���һ�����Ӿ�����ͷ����ʱ����Ұ���w��s������������Ӧ��
            if((tmp_direct == 'w' || tmp_direct == 's') && (direct == 'a' || direct == 'd')) 
                direct = tmp_direct;
            else if((tmp_direct == 'a' || tmp_direct == 'd') && (direct == 'w' || direct == 's')) 
                direct = tmp_direct;
        }
        
        //�����ߵ�λ��
        //�Ȱ���βλ�ñ���һ��
        int tmp_x = snakelast->x;
        int tmp_y = snakelast->y;
        pos* p = snakelast;//��ʱ�ڵ�p
        //��ǰһ���ڵ�����긳����һ���ڵ㼴��
        while(p->former){
            p->x = p->former->x;
            p->y = p->former->y;
            p = p->former;
        }
        //��һ�����õ�������Ч����˫��������ȫ��ͬ 
        // while (p != snakehead) {
        //     pos* q = snakehead;
        //     while (q->next != p) {
        //         q = q->next;
        //     }
        //     p->x = q->x; p->y = q->y;
        //     p = q;
        // }

        //���ݷ������ͷλ�ý����ƶ�
        if(direct == 'w') snakehead->y--;
        else if(direct == 'a') snakehead->x--;
        else if(direct == 's') snakehead->y++;
        else if(direct == 'd') snakehead->x++;

        //ײ��ǽ��ҧ����������Ϸ����
        if(map[snakehead->y][snakehead->x] == '#' || map[snakehead->y][snakehead->x] == 'o') {
            jumpCursor(0, WIDTH + 1);
            printf("Game Over!\n");
            break;
        }

        //��ӡ�ߣ���ɾ��β���ٴ���ͷ��ʼ��ӡ����
        jumpCursor(tmp_x, tmp_y);//�Ƚ���βɾ��
        printf(" ");
        map[tmp_y][tmp_x] = ' ';
        jumpCursor(snakehead->x, snakehead->y);//�ٴ�ӡ��ͷ
        printf("O");
        map[snakehead->y][snakehead->x] = 'O';
        pos* q = snakehead->next;
        while(q){
            jumpCursor(q->x, q->y);//��ӡ����
            printf("o");
            map[q->y][q->x] = 'o';
            q = q->next;
        }

        //��ӡ������Ҫ�ó���˯��һ�£���Ȼ����ִ��̫�죬�ߵ��˶���̫��
        //˯��ʱ��������ѡ����Ѷȶ������Ѷ�Խ�ߣ�˯��ʱ��Խ�̣��ߵ��˶���Խ��
        //�����Ұ�����j����˯��ʱ����Զ�һ�㣬�����ߵ��˶��ٶȾͻ���
        if(tmp_direct == 'j'){
            Sleep(10);
            tmp_direct = ' ';
        }else{
            Sleep(difficulty);
        }
        
        //�Ե�ʳ��(����Ϊʳ���λ�ñ��滻������ͷ)
        if(map[foody][foodx] == 'O'){
            //��ת����β��ʧ������ӡ����β
            jumpCursor(tmp_x, tmp_y);
            printf("o");
            map[tmp_y][tmp_x] = 'o';
            //������β��������
            pos* tmp_node = (pos*)malloc(sizeof(pos));
            tmp_node->x = tmp_x;
            tmp_node->y = tmp_y;
            tmp_node->former = snakelast;
            tmp_node->next = NULL;
            snakelast->next = tmp_node;
            snakelast = snakelast->next;
            //������ʳ��
            generateFood(&foodx, &foody);
            //��������
            score += 10;
            //���·�����ʾ
            jumpCursor(0, WIDTH);
            printf("��� %s ��ǰ����Ϊ�� %d", name, score);
        }
    }

    //��ȡ����ʱ��
    char time2[100];
    strcpy(time2, getTime());
    
    //�������֣�������ʱ����Ѷ�
    saveFile(name, score, time1, time2);

    //�����ַ���
    memset(time1, 0, sizeof(time1));
    memset(time2, 0, sizeof(time2));
    //�ͷ�����
    freeSnake();

    fflush(stdin);//������̻�����
    char again[100];
    //ѯ���Ƿ�����һ��
    while(1){
        printf("�Ƿ�����һ��? yes/no:\n");
        gets(again);
        if(strcmp(again, "yes") == 0) {
            system("cls");
            playGame();
        }
        else if(strcmp(again, "no") == 0){
            printf("��Ϸ����\n");
            system("cls");
            showMenu();
            break;
        }
        else{
            printf("��Ч����qwq\n");
        }
    }
}

void showMenu_B(){
    system("cls");
    //��ӡBѡ��Ĳ˵�
    for(int i = 0; i < len2; i++){
        printf("%s\n", menu_B[i]);
    }
    option = 2;//Ĭ���Ѷ��ǵڶ���
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
                    //��ɾ����ǰ��<< >>
                    jumpCursor(0, option);
                    printf("  ");
                    jumpCursor(strlen(menu_B[option]), option);
                    printf("  ");
                    option--;
                    //���ƶ�����һ��ѡ���ӡ<< >>
                    jumpCursor(0, option);
                    printf("<<");
                    jumpCursor(strlen(menu_B[option]), option);
                    printf(">>");
                }
                else{
                    //��ɾ����ǰ��<< >>
                    jumpCursor(0, option);
                    printf("  ");
                    jumpCursor(strlen(menu_B[option]), option);
                    printf("  ");
                    option = len2 - 2;
                    //���ƶ������һ��ѡ���ӡ<< >>
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
                    //��ɾ����ǰ��<< >>
                    jumpCursor(0, option);
                    printf("  ");
                    jumpCursor(strlen(menu_B[option]), option);
                    printf("  ");
                    option = 1;
                    //���ƶ�����һ��ѡ���ӡ<< >>
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
            printf("����ѡ��ɶ��\n");
            Sleep(1200);
            system("cls");
            showMenu_B();
    }
    printf("�Ѷȸ�����ϣ�\n");
    Sleep(1200);
    system("cls");
    showMenu();
}

void showMenu_C(){
    system("cls");
    //��ӡCѡ��Ĳ˵�
    for(int i = 0; i < len3; i++){
        printf("%s\n", menu_C[i]);
    }
    option = 1;//Ĭ���Ѷ��ǵ�һ��
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
                    //��ɾ����ǰ��<< >>
                    jumpCursor(0, option);
                    printf("  ");
                    jumpCursor(strlen(menu_C[option]), option);
                    printf("  ");
                    option--;
                    //���ƶ�����һ��ѡ���ӡ<< >>
                    jumpCursor(0, option);
                    printf("<<");
                    jumpCursor(strlen(menu_C[option]), option);
                    printf(">>");
                }
                else{
                    //��ɾ����ǰ��<< >>
                    jumpCursor(0, option);
                    printf("  ");
                    jumpCursor(strlen(menu_C[option]), option);
                    printf("  ");
                    option = len3 - 2;
                    //���ƶ������һ��ѡ���ӡ<< >>
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
                    //��ɾ����ǰ��<< >>
                    jumpCursor(0, option);
                    printf("  ");
                    jumpCursor(strlen(menu_C[option]), option);
                    printf("  ");
                    option = 1;
                    //���ƶ�����һ��ѡ���ӡ<< >>
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
            printf("����ѡ��ɶ��\n");
            Sleep(1200);
            system("cls");
            showMenu_C();
    }
    printf("���������а�\n");
    system("pause");
    system("cls");
    //��ȡ�����ݾ��ͷŵ�����������Ϸ����Ҫ������Щ����
    freeData();
    showMenu_C();
}

void rankTime(){
    //�����������˳��͵�����
    //ÿ���ú�һ���ڵ��fnextָ��ǰһ���ڵ�
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
    int k = 1;//��������ð������
    while(k == 1){
        k = 0;
        data* p = datahead;
        while(p->fnext){
            //�ȸ����Ѷȴ�С��������
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
            //�ٸ��ݷ�����С��������(�Ѷ���Ȳ��ܽ�������)
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
    int k = 1;//��������ð������
    while(k == 1){
        k = 0;
        data* p = datahead;
        while(p->fnext){
            //������������ĸ,���̽�������
            //�������������sortName����������
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
        printf("�û�����    %s\n",  datahead->fname);
        printf("�÷֣�      %d\n",  datahead->fscore);
        //�Ѷ�����ֵ��Ҫ����ת��Ϊ�ַ����Ա���������
        printf("�Ѷȣ�      %s\n",  transDifficultyToString(datahead->fdifficulty));
        printf("��ʼʱ�䣺  %s\n",  datahead->ftime1);
        printf("����ʱ�䣺  %s\n",  datahead->ftime2);
        printf("\n");
        datahead = datahead->fnext;
    }
}

void changeMapsize(){
    //����ʱ���ȺͿ�ȴ�һ�£��ж��Ƿ񳬹�����
    int tmp_length;
    int tmp_width;
    //Ҫ����Сֵ��ԭ������Ϊ����Ҫ�����ϰ����ͼ̫С�ϰ��ﲻ������
    printf("�������ͼ�ĳ��ȣ�(��С��%d�Ҵ���20)\n", max_len);
    scanf("%d", &tmp_length);
    printf("�������ͼ�Ŀ��: (��С��%d�Ҵ���20)\n", max_width);
    scanf("%d", &tmp_width);

    //����Ƿ�Խ��
    if(tmp_length >= max_len || tmp_width >= max_width 
    || tmp_length <= 20 || tmp_width <= 20){
        system("cls");
        printf("��ͼ�ߴ�ı�ʧ�ܣ�\n");
        Sleep(1200);
        showMenu();
    }
    else {
        LENGTH = tmp_length;
        WIDTH = tmp_width;
        system("cls");
        printf("��ͼ�ߴ�ı�ɹ���\n");
        printf("���ȱ�����Ϊ%d, ��ȱ�����Ϊ%d", LENGTH, WIDTH);
        Sleep(1200);
        showMenu();
    }
}

void initializeMap(){
    //��ӡ��ͼ���ܵ�ǽ
    memset(map, ' ', sizeof(map));
    for(int j = 0; j < LENGTH; j++) map[0][j] = '#';
    for(int i = 0; i < WIDTH; i++){
        map[i][0] = '#';
        map[i][LENGTH - 1] = '#';
    }
    for(int j = 0; j < LENGTH; j++) map[WIDTH - 1][j] = '#';

    //����Ѷ�Ϊ�����ߣ������ϰ������
    //������������һ����ͼ��������������
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
    //ʵ��ԭ������ȸ��ݵ�ͼ��Сȷ���ϰ��ﳤ�ȺͿ��
    //������ǽ���4���ո�ĵط���ӡ�ϰ���
    if(difficulty == veryhard){
        int olen = LENGTH / 2 / 2; //olen��ʾ�ϰ��ﳤ��
        int owid = WIDTH / 2 / 2;  //owid��ʾ�ϰ�����
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
    //��ʼ����Ϊ ooO
    //��ͷ
    snakehead = (pos*)malloc(sizeof(pos));
    snakehead->x = LENGTH / 2;
    snakehead->y = WIDTH / 2;
    snakehead->next = NULL;
    snakehead->former = NULL;
    //��һ������
    pos* body = (pos*)malloc(sizeof(pos));
    body->x = snakehead->x - 1; 
    body->y = snakehead->y;
    body->former = snakehead;
    body->next = NULL;
    snakehead->next = body;
    //�ڶ�������Ҳ�������һ�ڣ�
    snakelast = (pos*)malloc(sizeof(pos));
    snakelast->x = snakehead->x - 2; 
    snakelast->y = snakehead->y;
    snakelast->former = body;
    snakelast->next = NULL;
    body->next = snakelast;
}

void generateFood(int *foodx, int *foody){
    //�ڿո�ط�����ʳ��
    while(map[*foody][*foodx] != ' '){
        *foodx = rand() % LENGTH;
        *foody = rand() % WIDTH;
    }
    jumpCursor(*foodx, *foody);
    printf("*");
    map[*foody][*foodx] = '*';
}

char *getTime(){
    //��ȡ��ǰʱ�䲢����ʱ���ַ���
    time_t now;
    struct tm *local_time;
    static char time_str[100]; //ȷ������ַ����㹻���Դ洢��ʽ�����ʱ��
    memset(time_str, 0, sizeof(time_str));

    time(&now);
    local_time = localtime(&now);

    //ʹ�� snprintf �����⻺�������
    snprintf(time_str, sizeof(time_str), "%d-%02d-%02d %02d:%02d:%02d",
             local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
             local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

    return time_str;
}

void saveFile(char* name, int score, char* time1, char* time2){
    FILE* fptr = fopen("snakefile.txt", "a");//���ļ�׷��ģʽ
    if (!fptr) {
        printf("��ȡ�ļ�ʱ��������\n");
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
        printf("��ȡ�ļ�ʱ��������\n");
        return;
    }
    //�������֣������ȱ��������ڶ�ȡ�ļ��е�����
    char fname[100];
    int  fscore;
    int  fdifficulty;
    char ftime1[100];
    char ftime2[100];

    //�������ļ�ĩβʱfgets�����᷵��NULL
    while( fgets(fname, sizeof(fname), fptr) != NULL ){
        //fgets�������ȡ���з���Ҫ�����з�λ�øĳ�'\0'
        fname[strlen(fname) - 1] = '\0';
        //fgets������ȡ��һ�е����֣�fscanf������ȡ�ڶ��еĳɼ�
        //�Դ�����
        fscanf(fptr, "%d\n", &fscore);
        fscanf(fptr, "%d\n", &fdifficulty);
        fgets(ftime1, sizeof(ftime1), fptr); ftime1[strlen(ftime1) - 1] = '\0';
        fgets(ftime2, sizeof(ftime2), fptr); ftime2[strlen(ftime2) - 1] = '\0';

        //������ʱ�ڵ�tmp_node��malloc�ռ�֮��Ѷ��������ݶ�����ȥ
        //����Ϊʲô��ֱ�Ӹ�last->next����ռ䲢����ȥ������Ϊ����ʱ�ڵ��űȽ�ֱ��
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
        //������飬����������һ������
        memset(fname, 0, sizeof(fname));
        memset(ftime1, 0, sizeof(ftime1));
        memset(ftime2, 0, sizeof(ftime2));
    }
    //�ر��ļ�
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
    //���Ѷ���ֵת���ɶ�Ӧ���ַ���
    //���統difficulty = 150ʱ�������ַ��������ߡ�
    //����ɾ�̬ȫ�ֱ�������Ϊ�뿪�ú���ʱ��Щ�ַ�����Ҫ����
    static char d1[10] = "С��", d2[10] = "����", d3[10] = "����", d4[10] = "������";
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
    //������ͬ����������
    if(strcmp(name1, name2) == 0) return 0;

    //�������������������ĸС����ǰ���������̵ֶ���ǰ
    //���磺andon > andong > maoshun
    //�ȱȽ�����ĸ��С 
    int min = (strlen(name1) <= strlen(name2)) ? strlen(name1) : strlen(name2);
    for(int i = 0; i < min; i++){
        if(name1[i] < name2[i]){
            return 0;
        }
        else if(name1[i] > name2[i]){
            return 1;
        }
    }
    //����ĸ�Ƚϲ����ˣ��Ƚ����ֳ���
    if(strlen(name1) == min) return 0;
    else return 1;
}

void hideCursor(){
        HANDLE hConsoleOutput;
        CONSOLE_CURSOR_INFO cursorInfo;
        // ��ȡ��ǰ����̨����ľ��
        hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        // ��ȡ��ǰ�����Ϣ
        GetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
        // ���ù��Ŀɼ��ԣ��ڶ�������Ϊ0ʱ���ع��
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
}

void showCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = TRUE; // ���ù��ɼ�
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void jumpCursor(int x, int y){
	COORD pos; //������λ�õĽṹ�����
	pos.X = x; //������
	pos.Y = y; //������
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); //��ȡ����̨���
	SetConsoleCursorPosition(handle, pos); //���ù��λ��
}
