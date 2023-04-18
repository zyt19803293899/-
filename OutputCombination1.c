/*
    头一开始，我打算用数组存储机组特性资料，科哥偶然的一句要把数据存入txt文件中，在程序中调用数据文件让我改变了程序的做法，但是在编程的过程中，我并没有感受到用数据文件的简便，还不如用数组呢？于是乎，我产生了深深的疑问，数据文件那么难用，还得调用，有什么意义呢？
    我下意识背起了调用文件的好处：
        1.使得程序看起来更加简单
        2.解除了程序和数据之间的耦合，啥意思呢？也就是说我不用每回都改程序了，我只需要把文件改一下就行了。如果把文件的内容改一下，那就表示我换了一个机组；如果把文件的数量改一下呢？等等...
        把文件的数量改一下那不就意味着我可以优化任何数量的机组了吗？
        也就是说，我可以优化任何数量、任何类型机组了，换言之，程序的普适性更强了，后来琢磨了一下，就是n个阶段的动态规划问题。
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

bool findMinAndMaxLoad(int serialNum); // 找到最小和最大的负荷
void optimization(); // 优化
void onlyOneUnitWork(FILE *fp, char *charLoad, char *waterDemand); // 只有一个机组工作
void eitherOrUnitWork(); // 二选一机组工作
void bothUnitsWork(int greatAmbit, char *fileDirName1, int smallAmbit, char *fileDirName2); // 两台机组工作

int loadMin1, loadMin2, loadMax1, loadMax2, load, lineNum;
float watDemSum;
char fileDirName1[50], fileDirName2[50], fileDirName3[50], load1[15], load2[15], waterDemand1[15], waterDemand2[15];
FILE *fp1 = NULL, *fp2 = NULL, *fp3 = NULL, *fp4 = NULL, *fp5 = NULL;

int main() {
    int num;
    printf("Please enter the number of calculation stages(This means the number of files to enter):\n");
    scanf("%d", &num);
    for (int i = 1; i < num; i++) {
        if (i == 1) { // 第一阶段要读入两个文件
            printf("Please enter the absolute path of unit 1 characteristic data file(If the program and data files are in the same folder, you can also enter the relative path):\n");
            scanf("%s", fileDirName1);
            printf("Please enter the absolute path of unit 2 characteristic data file(If the program and data files are in the same folder, you can also enter the relative path):\n");
            scanf("%s", fileDirName2);
            if (findMinAndMaxLoad(1)) { // 当括号里面是true的时候，说明fp1是空，输入的文件名并不存在
                // return 0代表程序正常退出，return 1代表程序异常退出
                return 1;
            }
            if (findMinAndMaxLoad(2)) { // 同上
                return 1;
            }
            strcpy(fileDirName3, "Data1.txt");
            optimization();
        }
        else { // 非第一阶段只需读入一个文件，因为把生成文件的路径手动赋值了
            strcpy(fileDirName1, fileDirName3); // 把生成文件的路径赋给fileDirName1，而不用手动输入生成文件的路径赋值给fileDirName1
            // 把生成文件的最小负荷（loadMin1）和最大负荷（loadMax1+loadMax2）分别赋值给loadMin1和LoadMax1
            loadMax1 = loadMax1 + loadMax2;
            printf("Please enter the absolute path of unit %d characteristic data file(If the program and data files are in the same folder, you can also enter the relative path):\n", i + 1);
            scanf("%s", fileDirName2); // 我们用fileDirName2来存储新数据文件的路径
            if (findMinAndMaxLoad(2)) {
                return 1;
            }
            fileDirName3[4] = i + 48; // 用i来修改输出文件的文件名，不过需要注意fileDirName3是char类型的，这里还需要一点转化技巧，参考ASCII码表即可
            optimization();
        }
    }
}

bool findMinAndMaxLoad(int serialNum) {  // 找到最小负荷最大负荷
    lineNum = 0;
    if (serialNum == 1) { // 先判断序号，序号为1则分别将fileDirName1对应的最小负荷和最大负荷存到loadMin1和loadMax1中
        fp1 = fopen(fileDirName1, "r");
        if (NULL == fp1) {
            printf("open err!\n");
            return true;
        }
        while(fscanf(fp1, "%s%s", load1, waterDemand1) != EOF) {
            lineNum++;
            if (lineNum == 3) {
                loadMin1 = atoi(load1);
            }
        }
        loadMax1 = atoi(load1);
        fclose(fp1);
    }
    else { // 序号为2则分别将fileDirName2对应的最小负荷和最大负荷存到LoadMin2和loadMax2中
        fp2 = fopen(fileDirName2, "r");
        if (NULL == fp2) {
            printf("open err!\n");
            return true;
        }
        while(fscanf(fp2, "%s%s", load2, waterDemand2) != EOF) {
            lineNum++;
            if (lineNum == 3) {
                loadMin2 = atoi(load2);
            }
        }
        loadMax2 = atoi(load2);
        fclose(fp2); // 尽量把流关闭，因为流是占用内存的
    }
    return false;
}
void optimization() { // 对两个文件的特性资料进行优化，即求出所有可能负荷所对应的最小需水量
    fp1 = fopen(fileDirName1, "r");
    fp2 = fopen(fileDirName2, "r");
    fp3 = fopen(fileDirName3, "w");
    fprintf(fp3, "负荷    需水量\n");
    fprintf(fp3, "0       0\n");

    if (loadMax1 <= loadMax2 && loadMax2 <= loadMin1 + loadMin2) {
        for (load = loadMin1; load <= loadMax1 + loadMax2; load++) {
            if (load < loadMin2) { // 只有一台功率较小的机组工作
                onlyOneUnitWork(fp1, load1, waterDemand1);
                fprintf(fp3, "%-8d%s\n", load, waterDemand1);
            }
            else if (load <= loadMax1) {
                if (load < loadMax1) { // 两台都能进行工作，选需水量小的那个
                    eitherOrUnitWork();
                    fprintf(fp3, "%-8d%.1f\n", load, atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1));
                }
                else { // load == loadMax1
                    if (loadMax1 < loadMax2) {
                        eitherOrUnitWork();
                        fprintf(fp3, "%-8d%.1f\n", load, atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1));
                    }
                    else { // loadMax1 == loadMax2
                        if (loadMax2 < loadMin1 + loadMin2) {
                            eitherOrUnitWork();
                            fprintf(fp3, "%-8d%.1f\n", load, atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1));
                        }
                        else { // loadMax2 == loadMin1 + loadMin2
                            eitherOrUnitWork();
                            float temp1 = atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1);
                            bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                            fprintf(fp3, "%-8d%.1f\n", load, temp1>watDemSum?watDemSum:temp1);
                        }
                        fclose(fp2);
                    }
                    fclose(fp1);
                }
            }
            else if (load <= loadMax2) {
                if (load < loadMax2) { // 功率较大的一个机组工作
                    onlyOneUnitWork(fp2, load2, waterDemand2);
                    fprintf(fp3, "%-8d%s\n", load, waterDemand2);
                }
                else { // load == loadMax2
                    if (loadMax2 < loadMin1 + loadMin2) { // 功率较大的一个机组工作
                        onlyOneUnitWork(fp2, load2, waterDemand2);
                        fprintf(fp3, "%-8d%s\n", load, waterDemand2);
                    }
                    else { // loadMax2 == loadMin1 + loadMin2 功率较大的一个机组或共同
                        onlyOneUnitWork(fp2, load2, waterDemand2);
                        float temp1 = atof(waterDemand2);
                        bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                        fprintf(fp3, "%-8d%.1f\n", load, temp1>watDemSum?watDemSum:temp1);
                    }
                    fclose(fp2);
                }
            }
            else if (load < loadMin1 + loadMin2) {
                // 啥也不干
            }
            else { // 两台机组共同工作
                if (loadMax1 - loadMin1 > loadMax2 - loadMin2) {
                    bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                    fprintf(fp3, "%-8d%.1f\n", load, watDemSum);
                }
                else {
                    bothUnitsWork(loadMax2-loadMin2, fileDirName2, loadMax1-loadMin1, fileDirName1);
                    fprintf(fp3, "%-8d%.1f\n", load, watDemSum);
                }
            }
        }
    }
    else if (loadMax2 < loadMax1 && loadMax1 <= loadMin1 + loadMin2) {
        // 注意，第一个式子没有写成loadMax2==loadMax1，因为第一个if包含了这种情况
        for (load = loadMin1; load <= loadMax1 + loadMax2; load++) {
            if (load < loadMin2) { // 只有一台功率较小的机组工作
                onlyOneUnitWork(fp1, load1, waterDemand1);
                fprintf(fp3, "%-8d%s\n", load, waterDemand1);
            }
            else if (load <= loadMax2) {
                eitherOrUnitWork();
                fprintf(fp3, "%-8d%.1f\n", load, atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1));
                if (load == loadMax2) {
                    fclose(fp2);
                }
            }
            else if (load <= loadMax1) {
                if (load < loadMax1) { // 只有一台功率较大的机组工作
                    onlyOneUnitWork(fp1, load1, waterDemand1);
                    fprintf(fp3, "%-8d%s\n", load, waterDemand1);
                }
                else { // load == loadMax1
                    if (loadMax1 < loadMin1 + loadMin2) {
                        onlyOneUnitWork(fp1, load1, waterDemand1);
                        fprintf(fp3, "%-8d%s\n", load, waterDemand1);
                    }
                    else { // loadMax1 == loadMin1 + loadMin2 一台功率较大或共同
                        onlyOneUnitWork(fp1, load1, waterDemand1);
                        float temp1 = atof(waterDemand1);
                        bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                        fprintf(fp3, "%-8d%.1f\n", load, temp1>watDemSum?watDemSum:temp1);
                    }
                    fclose(fp1);
                }
            }
            else if (load < loadMin1 + loadMin2) { // 中间可能出现断层的现象
                // 什么也不用干
            }
            else { // 两台机组共同工作，对于这种情况，loadMax1-loadMin1一定大于loadMax2-loadMin2的
                bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                fprintf(fp3, "%-8d%.1f\n", load, watDemSum);
            }
        }
    }
    else if (loadMax1 <= loadMin1 + loadMin2 && loadMin1 + loadMin2 < loadMax2) {
        // 注意，第二个式子没有写成loadMin1+loadMin2<=loadMax2，因为第一个if包含了这种情况
        for (load = loadMin1; load <= loadMax1 + loadMax2; load++) {
            if (load < loadMin2) { // 只有一台功率较小的机组工作
                onlyOneUnitWork(fp1, load1, waterDemand1);
                fprintf(fp3, "%-8d%s\n", load, waterDemand1);
            }
            else if (load <= loadMax1) { // 两台机组都能进行工作
                if (load < loadMax1) {
                    eitherOrUnitWork();
                    fprintf(fp3, "%-8d%.1f\n", load, atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1));
                }
                else { // load == loadMax1
                    if (loadMax1 < loadMin1 + loadMin2) { // 二选一
                        eitherOrUnitWork();
                        fprintf(fp3, "%-8d%.1f\n", load, atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1));
                    }
                    else { // loadMax1 == loadMin1 + loadMin2 二选一或共同工作
                        eitherOrUnitWork();
                        float temp1 = atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1);
                        bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                        fprintf(fp3, "%-8d%.1f\n", load, temp1>watDemSum?watDemSum:temp1);
                    }
                    fclose(fp1);
                }
            }
            else if (load < loadMin1 + loadMin2) { // 这种情况有可能发生
                onlyOneUnitWork(fp2, load2, waterDemand2);
                fprintf(fp3, "%-8d%s\n", load, waterDemand2);
            }
            else if (load <= loadMax2) { // 选择两台机组或一台功率较大的机组在相同负荷下的最小需水量
                // 这个必须判断loadMax1-loadMin1和loadMax2-loadMin2谁大谁小，原因参考311行
                if (loadMax1 - loadMin1 > loadMax2 - loadMin2) {
                    bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                }
                else {
                    bothUnitsWork(loadMax2-loadMin2, fileDirName2, loadMax1-loadMin1, fileDirName1);
                }
                onlyOneUnitWork(fp2, load2, waterDemand2);
                float temp1 = atof(waterDemand2);
                fprintf(fp3, "%-8d%.1f\n", load, temp1>watDemSum?watDemSum:temp1);
                if (load == loadMax2) {
                    fclose(fp2);
                }
            }
            else { // 两台机组共同工作
                if (loadMax1 - loadMin1 > loadMax2 - loadMin2) {
                    bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                    fprintf(fp3, "%-8d%.1f\n", load, watDemSum);
                }
                else {
                    bothUnitsWork(loadMax2-loadMin2, fileDirName2, loadMax1-loadMin1, fileDirName1);
                    fprintf(fp3, "%-8d%.1f\n", load, watDemSum);
                }
            }
        }
    }
    else if (loadMax2 <= loadMin1 + loadMin2 && loadMin1 + loadMin2 < loadMax1) {
        // 注意，式子中的第二项没有写成loadMin1+loadMin2<=loadMax1，第二个else if包含了这种情况
        for (load = loadMin1; load <= loadMax1 + loadMax2; load++) {
            if (load < loadMin2) { // 只有一台功率较小的机组工作
                onlyOneUnitWork(fp1, load1, waterDemand1);
                fprintf(fp3, "%-8d%s\n", load, waterDemand1);
            }
            else if (load <= loadMax2) {
                if (load < loadMax2) {
                    eitherOrUnitWork();
                    fprintf(fp3, "%-8d%.1f\n", load, atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1));
                }
                else { // load == loadMax2
                    if (loadMax2 < loadMin1 + loadMin2) { // 二选一
                        eitherOrUnitWork();
                        fprintf(fp3, "%-8d%.1f\n", load, atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1));
                    }
                    else { // loadMax2 == loadMin1 + loadMin2 二选一或共同
                        eitherOrUnitWork();
                        float temp1 = atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1);
                        bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                        fprintf(fp3, "%-8d%.1f\n", load, temp1>watDemSum?watDemSum:temp1);
                    }
                    fclose(fp2);
                }
            }
            else if (load < loadMin1 + loadMin2) { // 只有一台功率较大的机组工作
                onlyOneUnitWork(fp1, load1, waterDemand1);
                fprintf(fp3, "%-8d%s\n", load, waterDemand1);
            }
            else if (load <= loadMax1) { // 选择两台机组或一台功率较大的机组在相同负荷下的最小需水量
                // 可以确定loadMax1-loadMin1>loadMax2-loadMin2
                onlyOneUnitWork(fp1, load1, waterDemand1);
                float temp1 = atof(waterDemand1);
                bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                fprintf(fp3, "%-8d%.1f\n", load, temp1>watDemSum?watDemSum:temp1);
                if (load == loadMax1) {
                    fclose(fp1);
                }
            }
            else { // 两台机组工作
                bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                fprintf(fp3, "%-8d%.1f\n", load, watDemSum);
            }
        }
    }
    else if (loadMin1 + loadMin2 < loadMax1 && loadMax1 <= loadMax2) {
        // 注意，式子中的第一项没有写为loadMin1+loadMin2<=loadMax1，第三个else if包含了这种情况
        for (load = loadMin1; load <= loadMax1 + loadMax2; load++) {
            if (load < loadMin2) { // 只有一台功率较小的机组工作
                onlyOneUnitWork(fp1, load1, waterDemand1);
                fprintf(fp3, "%-8d%s\n", load, waterDemand1);
            }
            else if (load < loadMin1 + loadMin2) { // 二选一
                eitherOrUnitWork();
                fprintf(fp3, "%-8d%.1f\n", load, atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1));
            }
            else if (load <= loadMax1) { // 两台机组可以单干，也可以共同干
                eitherOrUnitWork();
                float temp1 = atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1);
                // 这里说明一下，为什么bothUnitsWork没有判断loadMax1-loadMin1和loadMax2-loadMin2谁大谁小。在这个范围内，load最大取值为loadMax1，而lineNum=load-loadMin1-loadMin2<=loadMax1-loadMin1-loadMin2，而loadMax1-loadMin1-loadMin2既小于loadMax1-loadMin1，也小于loadMax2-loadMin2。啥意思呢？lineNum一定小于smallAmbit，这就是说，对lineNum的分配不会越界，可以随便分配。
                bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                fprintf(fp3, "%-8d%.1f\n", load, temp1>watDemSum?watDemSum:temp1);
                if (load == loadMax1) {
                    fclose(fp1);
                    if (loadMax1 == loadMax2) {
                        fclose(fp2);
                    }
                }
            }
            else if (load <= loadMax2) { // 功率较大的机组单干或共同干
                onlyOneUnitWork(fp2, load2, waterDemand2);
                float temp1 = atof(waterDemand2);
                // 参考311行，这个就没有办法保证lineNum不会越界
                if (loadMax1 - loadMin1 > loadMax2 - loadMin2) {
                    bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                }
                else {
                    bothUnitsWork(loadMax2-loadMin2, fileDirName2, loadMax1-loadMin1, fileDirName1);
                }
                fprintf(fp3, "%-8d%.1f\n", load, temp1>watDemSum?watDemSum:temp1);
                if (load == loadMax2) {
                    fclose(fp2);
                }
            }
            else { // 共同干
                if (loadMax1 - loadMin1 > loadMax2 - loadMin2) {
                    bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                    fprintf(fp3, "%-8d%.1f\n", load, watDemSum);
                }
                else {
                    bothUnitsWork(loadMax2-loadMin2, fileDirName2, loadMax1-loadMin1, fileDirName1);
                    fprintf(fp3, "%-8d%.1f\n", load, watDemSum);
                }
            }
        }
    }
    else { // loadMin1 + loadMin2 < loadMax2 && loadMax2 < loadMax1
        // 我们把每种情况没有重复地进行了分类，并行不悖
        for (load = loadMin1; load <= loadMax1 + loadMax2; load++) {
            if (load < loadMin2) { // 只有一台功率较小的机组工作
                onlyOneUnitWork(fp1, load1, waterDemand1);
                fprintf(fp3, "%-8d%s\n", load, waterDemand1);
            }
            else if (load < loadMin1 + loadMin2) { // 二选一
                eitherOrUnitWork();
                fprintf(fp3, "%-8d%.1f\n", load, atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1));
            }
            else if (load <= loadMax2) { // 两台机组可以单干，也可以共同干
                eitherOrUnitWork();
                float temp1 = atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1);
                bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                fprintf(fp3, "%-8d%.1f\n", load, temp1>watDemSum?watDemSum:temp1);
                if (load == loadMax2) {
                    fclose(fp2);
                }
            }
            else if (load <= loadMax1) {
                onlyOneUnitWork(fp1, load1, waterDemand1);
                float temp1 = atof(waterDemand1);
                bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                fprintf(fp3, "%-8d%.1f\n", load, temp1>watDemSum?watDemSum:temp1);
                if (load == loadMax1) {
                    fclose(fp1);
                }
            }
            else {
                bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                fprintf(fp3, "%-8d%.1f\n", load, watDemSum);
            }
        }
    }
    fclose(fp3);
}
void onlyOneUnitWork(FILE *fp, char *charLoad, char *waterDemand) { // 只有一台机组能工作
    while (fscanf(fp, "%s%s", charLoad, waterDemand) != EOF) {
        if (load == atoi(charLoad)) {
            break;
        }
    }
}
void eitherOrUnitWork() { // 两台机组都能进行工作，但选择相同负荷下需水量小的机组进行工作
    while (fscanf(fp1, "%s%s", load1, waterDemand1) != EOF) {
        if (load == atoi(load1)) {
            while (fscanf(fp2, "%s%s", load2, waterDemand2) != EOF) {
                if (load == atoi(load2)) {
                    break;
                }
            }
            break;
        }
    }
}
void bothUnitsWork(int greatAmbit, char *fileDirName1, int smallAmbit, char *fileDirName2) { // 两台机组共同工作
    lineNum = load - loadMin1 - loadMin2;
    if (lineNum <= smallAmbit) {
        for (int i = 0; i <= lineNum; i++) { // 外圈控制是否结束
            fp4 = fopen(fileDirName1, "r");
            fp5 = fopen(fileDirName2, "r");
            for (int j = 0; j <= i + 2; j++) {
                fscanf(fp4, "%s%s", load1, waterDemand1);
            }
            for (int j = 0; j <= lineNum - i + 2; j++) {
                fscanf(fp5, "%s%s", load2, waterDemand2);
            }
            fclose(fp4);
            fclose(fp5);
            if (i == 0) {
                watDemSum = atof(waterDemand1) + atof(waterDemand2);
            }
            else {
                if (watDemSum > atof(waterDemand1) + atof(waterDemand2)) {
                    watDemSum = atof(waterDemand1) + atof(waterDemand2);
                }
            }
        }
    }
    else if (lineNum <= greatAmbit) {
        for (int i = 0; i <= smallAmbit; i++) {
            fp4 = fopen(fileDirName1, "r");
            fp5 = fopen(fileDirName2, "r");
            for (int j = 0; j <= lineNum - smallAmbit + i + 2; j++) {
                fscanf(fp4, "%s%s", load1, waterDemand1);
            }
            for (int j = 0; j <= smallAmbit - i + 2; j++) {
                fscanf(fp5, "%s%s", load2, waterDemand2);
            }
            fclose(fp4);
            fclose(fp5);
            if (i == 0) {
                watDemSum = atof(waterDemand1) + atof(waterDemand2);
            }
            else {
                if (watDemSum > atof(waterDemand1) + atof(waterDemand2)) {
                    watDemSum = atof(waterDemand1) + atof(waterDemand2);
                }
            }
        }
    }
    else {
        for (int i = 0; i <= greatAmbit + smallAmbit - lineNum; i++) {
            fp4 = fopen(fileDirName1, "r");
            fp5 = fopen(fileDirName2, "r");
            for (int j = 0; j <= lineNum - smallAmbit + i + 2; j++) {
                fscanf(fp4, "%s%s", load1, waterDemand1);
            }
            for (int j = 0; j <= smallAmbit - i + 2; j++) {
                fscanf(fp5, "%s%s", load2, waterDemand2);
            }
            fclose(fp4);
            fclose(fp5);
            if (i == 0) {
                watDemSum = atof(waterDemand1) + atof(waterDemand2);
            }
            else {
                if (watDemSum > atof(waterDemand1) + atof(waterDemand2)) {
                    watDemSum = atof(waterDemand1) + atof(waterDemand2);
                }
            }
        }
    }
}