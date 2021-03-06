//
// Created by 密码123 on 16/3/1.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

#include <iostream>
#include <fstream>
#include <queue>


//haffman 树的结构
typedef struct {
    //叶子结点权值
    unsigned int weight;
    //指向双亲，和孩子结点的指针
    unsigned int parent;
    unsigned int lChild;
    unsigned int rChild;
    char data;
} Node, *HuffmanTree;

//保存树的权值和数据的数组
typedef struct {
    int *w;
    char *a;
} HNode;


//动态分配数组，存储哈夫曼编码
typedef char *HuffmanCode;
HuffmanCode *huffmanCode;

string rootPath = "/Users/mima123/ClionProjects/huffmanApp_c/files/";
//要被转换为01代码的原始文本文件
string tobeTransPath = rootPath + "tobetrans.txt";
//存放01代码的文本文件
string codefilePath = rootPath + "codefile";
//存放转码后文本的文件
string decodefilePath = rootPath + "textfile";
//存放字符及对应的01代码
string codingFilePath = rootPath + "coding.dat";
//压缩文件
string compressFilePath = rootPath + "compress";
//解压缩后的文本文件
string decompressFilePah = rootPath + "decompress";

//选择两个parent为0，且weight最小的结点s1和s2的方法实现
//n 为叶子结点的总数，s1和 s2两个指针参数指向要选取出来的两个权值最小的结点
void Select(HuffmanTree *huffmanTree, int n, int *s1, int *s2) {
    //标记 i
    int i = 0;
    //记录最小权值
    int min;
    //遍历全部结点，找出单节点
    for (i = 1; i <= n; i++) {
        //如果此结点的父亲没有，那么把结点号赋值给 min，跳出循环
        if ((*huffmanTree)[i].parent == 0) {
            min = i;
            break;
        }
    }
    //继续遍历全部结点，找出权值最小的单节点
    for (i = 1; i <= n; i++) {
        //如果此结点的父亲为空，则进入 if
        if ((*huffmanTree)[i].parent == 0) {
            //如果此结点的权值比 min 结点的权值小，那么更新 min 结点，否则就是最开始的 min
            if ((*huffmanTree)[i].weight < (*huffmanTree)[min].weight) {
                min = i;
            }
        }
    }
    //找到了最小权值的结点，s1指向
    *s1 = min;
    //遍历全部结点
    for (i = 1; i <= n; i++) {
        //找出下一个单节点，且没有被 s1指向，那么i 赋值给 min，跳出循环
        if ((*huffmanTree)[i].parent == 0 && i != (*s1)) {
            min = i;
            break;
        }
    }
    //继续遍历全部结点，找到权值最小的那一个
    for (i = 1; i <= n; i++) {
        if ((*huffmanTree)[i].parent == 0 && i != (*s1)) {
            //如果此结点的权值比 min 结点的权值小，那么更新 min 结点，否则就是最开始的 min
            if ((*huffmanTree)[i].weight < (*huffmanTree)[min].weight) {
                min = i;
            }
        }
    }
    //s2指针指向第二个权值最小的叶子结点
    *s2 = min;
}

//创建哈夫曼树并求哈夫曼编码的算法如下，w数组存放已知的n个权值
void createHuffmanTree(HuffmanTree *huffmanTree, int w[], char a[], int n) {
    //m 为哈夫曼树总共的结点数，n 为叶子结点数
    int m = 2 * n - 1;
    //s1 和 s2 为两个当前结点里，要选取的最小权值的结点
    int s1;
    int s2;
    //标记
    int i;
    // 创建哈夫曼树的结点所需的空间，m+1，代表其中包含一个头结点
    *huffmanTree = (HuffmanTree) malloc((m + 1) * sizeof(Node));
    //1--n号存放叶子结点，初始化叶子结点，结构数组来初始化每个叶子结点，初始的时候看做一个个单个结点的二叉树
    for (i = 1; i <= n; i++) {

        //其中叶子结点的权值是 w【n】数组来保存
        (*huffmanTree)[i].weight = w[i];
        //为叶子节点赋值
        (*huffmanTree)[i].data = a[i];
        //初始化叶子结点（单个结点二叉树）的孩子和双亲，单个结点，也就是没有孩子和双亲，==0
        (*huffmanTree)[i].lChild = 0;
        (*huffmanTree)[i].parent = 0;
        (*huffmanTree)[i].rChild = 0;
    }// end of for
    //非叶子结点的初始化
    for (i = n + 1; i <= m; i++) {
        (*huffmanTree)[i].weight = 0;
        (*huffmanTree)[i].lChild = 0;
        (*huffmanTree)[i].parent = 0;
        (*huffmanTree)[i].rChild = 0;
    }

    printf("\n HuffmanTree: \n");
    //创建非叶子结点，建哈夫曼树
    for (i = n + 1; i <= m; i++) {
        //在(*huffmanTree)[1]~(*huffmanTree)[i-1]的范围内选择两个parent为0
        //且weight最小的结点，其序号分别赋值给s1、s2
        Select(huffmanTree, i - 1, &s1, &s2);
        //选出的两个权值最小的叶子结点，组成一个新的二叉树，根为 i 结点
        (*huffmanTree)[s1].parent = i;
        (*huffmanTree)[s2].parent = i;
        (*huffmanTree)[i].lChild = s1;
        (*huffmanTree)[i].rChild = s2;
        //新的结点 i 的权值
        (*huffmanTree)[i].weight = (*huffmanTree)[s1].weight + (*huffmanTree)[s2].weight;
        (*huffmanTree)[i].data = '0'; //为非叶子节点赋默认值

        printf("%d (%d, %d)\n", (*huffmanTree)[i].weight, (*huffmanTree)[s1].weight, (*huffmanTree)[s2].weight);
    }

    printf("\n");
}

//哈夫曼树建立完毕，从 n 个叶子结点到根，逆向求每个叶子结点对应的哈夫曼编码
void creatHuffmanCode(HuffmanTree *huffmanTree, int n) {
    //指示biaoji
    int i;
    //编码的起始指针
    int start;
    //指向当前结点的父节点
    int p;
    //遍历 n 个叶子结点的指示标记 c
    unsigned int c;
    //分配n个编码的头指针
    huffmanCode = (HuffmanCode *) malloc((n + 1) * sizeof(char *));
    //分配求当前编码的工作空间
    char *cd = (char *) malloc(n * sizeof(char));
    //从右向左逐位存放编码，首先存放编码结束符
    cd[n - 1] = '\0';
    //求n个叶子结点对应的哈夫曼编码
    for (i = 1; i <= n; i++) {
        //初始化编码起始指针
        start = n - 1;
        //从叶子到根结点求编码
        for (c = i, p = (*huffmanTree)[i].parent; p != 0; c = p, p = (*huffmanTree)[p].parent) {
            if ((*huffmanTree)[p].lChild == c) {
                //从右到左的顺序编码入数组内
                cd[--start] = '0';  //左分支标0
            }
            else {
                cd[--start] = '1';  //右分支标1
            }
        }// end of for
        //为第i个编码分配空间
        huffmanCode[i] = (char *) malloc((n - start) * sizeof(char));

        strcpy(huffmanCode[i], &cd[start]);
    }

    free(cd);
    //打印编码序列

    //建立文件输出流
    ofstream ofile;
    //打开文件
    ofile.open("/Users/mima123/ClionProjects/huffmanApp_c/files/coding.dat");
//    ofile.open("/home/weixian/ClionProjects/huffmanApp_c/files/coding.dat");
//    ofile.open("../files/coding.dat", ios::out);

    for (i = 1; i <= n; i++) {
        printf("HuffmanCode of %3d is %s, data: %c.\n", (*huffmanTree)[i].weight, huffmanCode[i],
               (*huffmanTree)[i].data);
        //向文件写入字符及对应的编码
        ofile << (*huffmanTree)[i].data << " " << huffmanCode[i] << endl;
    }

    printf("\n");
}

/**
 * 对文件进行编码
 *
 * 参数说明:
 *       huffmanTree -- 哈弗曼树
 *       n -- 叶节点的个数,就是函数字符的节点个数
 */
void codeFile(HuffmanTree *huffmanTree, int n) {

    fstream ifile; //文件输入流
    fstream ofile; //文件输出流
    ifile.open("/Users/mima123/ClionProjects/huffmanApp_c/files/tobetrans.txt", ios::in | ios::binary);
    ofile.open("/Users/mima123/ClionProjects/huffmanApp_c/files/codefile", ios::out | ios::binary);
    string temp;
    cout << "codeFile方法里打印编码文件:" << endl;
    while (getline(ifile, temp)) {
        for (int i = 0; i < temp.length(); ++i) {
            char c = temp[i];
            for (int j = 1; j <= n; j++) {
                char data = (*huffmanTree)[j].data;
                if (c != 0) if (c == data) {
                    ofile << huffmanCode[j];
                    cout << huffmanCode[j];
                    break;
                }
            }
        }
        ofile << endl;
    }
    ifile.close();
    ofile.close();
}

int tot = 0;
char mystr[15000];


/**
 * 将01代码进行压缩
 */
void compress() {
    ifstream fp1; //存放
    ofstream fp2;
    fp1.open(codefilePath);
    if (!fp1)//检查文件是否打开。
    {
        cerr << "不能打开 " << codefilePath << "文件" << endl;//输出文件未打开的标志。
        exit(0);
    }
//    cout<<"输入文件名用来存放压缩后的文件"<<endl<<"例如huffman3.txt"<<endl;
//    cin>>s2;
    char inchar, ch;
    fp2.open(compressFilePath);
    if (!fp2)//检查文件是否打开。
    {
        cerr << "不能打开 " << compressFilePath << "文件" << endl;//输出文件未打开的标志。
        exit(0);
    }
    int t = 0, f = 0, s;
    //char a[8];
    while (!fp1.eof()) {
        fp1 >> inchar;
        s = inchar - '0';
        t *= 2;
        t += s;
        f++;
        if (f == 7) {
            ch = t;
            t = 0;
            fp2 << ch;
            cout << ch;
            mystr[tot++] = ch;
            f = 0;
        }
    }
    mystr[tot] = '\0';
    fp1.close();
    fp2.close();
}


queue<int> s;

void decompress(HuffmanTree *huffman, int n) {
    ifstream fp1;
    ofstream fp2;
    fp1.open(compressFilePath);
    if (!fp1)//检查文件是否打开。
    {
        cerr << "不能打开 " << compressFilePath << "文件" << endl;//输出文件未打开的标志。
        exit(0);
    }
//    cout<<"输入文件名用来存放解压后的文件"<<endl<<"例如huffman4.txt"<<endl;
//    cin>>s3;
    fp2.open(decompressFilePah);
    if (!fp2)//检查文件是否打开。
    {
        cout << "不能打开 " << decompressFilePah << "文件" << endl;//输出文件未打开的标志。
        exit(0);
    }
    int inchar;
    int i = 0;
    while (!s.empty())s.pop();
    for (int ptr = n; i < tot;) {
        if (s.size() < 3) {
            char ch;
            int num, a[8];
            fp1 >> ch;
            ch = mystr[i++];
            num = ch;
            for (int i = 6; i >= 0; i--) {
                a[i] = num % 2;
                num /= 2;
            }

            for (int i = 0; i < 7; i++) {
                //ch=a[i]+'0';
                s.push(a[i]);
            }
        }
        inchar = s.front();
        s.pop();
        if (inchar == 1)ptr = huffman[ptr]->rChild;//查找相应编码对应huffman树中的位置，
        else ptr = huffman[ptr]->lChild;
        if (huffman[ptr]->lChild == 0 && huffman[ptr]->rChild == 0)//判断是否为叶子结点；
        {
            fp2 << huffman[ptr]->data;
            ptr = n;
        }//是叶子结点，将该结点的对应字符输入到文件中；
    }
    fp1.close();
    fp2.close();
}



void decodeFile(HuffmanTree *huffmanTree) {
    ifstream ifile; //文件输入流
    ofstream ofile; //文件输出流
    ifile.open("/Users/mima123/ClionProjects/huffmanApp_c/files/codefile");
    ofile.open("/Users/mima123/ClionProjects/huffmanApp_c/files/textfile");
    string temp;
    cout << "decodeFile方法打印编码文件:" << endl;
    while (!ifile.eof()) {

    }
}

/**
 * 从原始原件获取字符的个数,种类,权值
 */
void getInput(HNode &node, int &n) {
    ifstream ifile; //建立文件输入流
    ifile.open("/Users/mima123/ClionProjects/huffmanApp_c/files/tobetrans.txt");
//    ifile.open("/home/weixian/ClionProjects/huffmanApp_c/files/testfile");
    string temp;
    int count = 0;
    int maxLength = 256;

    while (getline(ifile, temp)) {
        count += temp.size();
    }
    cout << count << endl;
    if (maxLength > count)
        maxLength = count;
    node.w = (int *) malloc((maxLength + 1) * sizeof(int));
    node.a = (char *) malloc((maxLength + 1) * sizeof(char));
    for (int i = 0; i < maxLength; i++) {
        node.w[i] = 0;
        node.a[i] = 0;
    }
    ifile.clear();
    ifile.seekg(0, ios::beg);
    while (getline(ifile, temp)) {
        cout << temp << endl;
        for (int i = 0; i < temp.size(); ++i) {
            for (int j = 0; j < maxLength; j++) {
                if (node.a[j] != 0) {
                    if (node.a[j] != temp[i])
                        continue;
                    else {
                        node.w[j]++;
                        break;
                    }

                } else {
                    node.a[j] = temp[i];
                    node.w[j]++;
                    break;
                }
            }
        }
    }
    for (int k = 0; k < maxLength && node.w[k] != 0; k++) {
        n++;
        cout << node.w[k] << " ";
    }
    cout << n << endl;
    ifile.close();
}


int main() {
    HuffmanTree HT;
    HuffmanCode HC;
    int i = 0, n = 0, wei, m;
    char da;

//    cout << "请输入字符集的大小：" << endl << "n = ";
//
//    scanf("%d", &n);
//
//    w = (int *) malloc((n + 1) * sizeof(int));
//    a = (char *) malloc((n + 1) * sizeof(char));
//
//    printf("\ninput the %d element's weight:\n", n);
//
//    cout << "请输入数据:" << endl;
//    for (i = 1; i <= n; i++) {
//        printf("%d: ", i);
////        fflush(stdin);
////        scanf("%c", &da);
//        cin >> da;
//        a[i] = da;
//    }
//
//    cout << "请输入权值:" << endl;
//    for (i = 1; i <= n; i++) {
//        printf("%d: ", i);
//        fflush(stdin);
//        scanf("%d", &wei);
//        w[i] = wei;
//    }

    HNode node;
    getInput(node, n);
    cout << "n: " << n << endl;
    for (int k = 0; k < n; k++) {
        cout << "w: " << node.w[k] << "  " << "a: " << node.a[k] << endl;
    }
    createHuffmanTree(&HT, node.w, node.a, n);
    creatHuffmanCode(&HT, n);
    codeFile(&HT, n);
    cout << endl << "compress" << endl;
    compress();
    cout << endl << "decompress" << endl;

    decompress(&HT, n);

    delete[]node.a;
    delete[]node.w;
    return 0;
}