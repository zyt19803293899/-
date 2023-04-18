/*
    ͷһ��ʼ���Ҵ���������洢�����������ϣ��Ƹ�żȻ��һ��Ҫ�����ݴ���txt�ļ��У��ڳ����е��������ļ����Ҹı��˳���������������ڱ�̵Ĺ����У��Ҳ�û�и��ܵ��������ļ��ļ�㣬�������������أ����Ǻ����Ҳ�������������ʣ������ļ���ô���ã����õ��ã���ʲô�����أ�
    ������ʶ�����˵����ļ��ĺô���
        1.ʹ�ó����������Ӽ�
        2.����˳��������֮�����ϣ�ɶ��˼�أ�Ҳ����˵�Ҳ���ÿ�ض��ĳ����ˣ���ֻ��Ҫ���ļ���һ�¾����ˡ�������ļ������ݸ�һ�£��Ǿͱ�ʾ�һ���һ�����飻������ļ���������һ���أ��ȵ�...
        ���ļ���������һ���ǲ�����ζ���ҿ����Ż��κ������Ļ�������
        Ҳ����˵���ҿ����Ż��κ��������κ����ͻ����ˣ�����֮������������Ը�ǿ�ˣ�������ĥ��һ�£�����n���׶εĶ�̬�滮���⡣
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

bool findMinAndMaxLoad(int serialNum); // �ҵ���С�����ĸ���
void optimization(); // �Ż�
void onlyOneUnitWork(FILE *fp, char *charLoad, char *waterDemand); // ֻ��һ�����鹤��
void eitherOrUnitWork(); // ��ѡһ���鹤��
void bothUnitsWork(int greatAmbit, char *fileDirName1, int smallAmbit, char *fileDirName2); // ��̨���鹤��

int loadMin1, loadMin2, loadMax1, loadMax2, load, lineNum;
float watDemSum;
char fileDirName1[50], fileDirName2[50], fileDirName3[50], load1[15], load2[15], waterDemand1[15], waterDemand2[15];
FILE *fp1 = NULL, *fp2 = NULL, *fp3 = NULL, *fp4 = NULL, *fp5 = NULL;

int main() {
    int num;
    printf("Please enter the number of calculation stages(This means the number of files to enter):\n");
    scanf("%d", &num);
    for (int i = 1; i < num; i++) {
        if (i == 1) { // ��һ�׶�Ҫ���������ļ�
            printf("Please enter the absolute path of unit 1 characteristic data file(If the program and data files are in the same folder, you can also enter the relative path):\n");
            scanf("%s", fileDirName1);
            printf("Please enter the absolute path of unit 2 characteristic data file(If the program and data files are in the same folder, you can also enter the relative path):\n");
            scanf("%s", fileDirName2);
            if (findMinAndMaxLoad(1)) { // ������������true��ʱ��˵��fp1�ǿգ�������ļ�����������
                // return 0������������˳���return 1��������쳣�˳�
                return 1;
            }
            if (findMinAndMaxLoad(2)) { // ͬ��
                return 1;
            }
            strcpy(fileDirName3, "Data1.txt");
            optimization();
        }
        else { // �ǵ�һ�׶�ֻ�����һ���ļ�����Ϊ�������ļ���·���ֶ���ֵ��
            strcpy(fileDirName1, fileDirName3); // �������ļ���·������fileDirName1���������ֶ����������ļ���·����ֵ��fileDirName1
            // �������ļ�����С���ɣ�loadMin1������󸺺ɣ�loadMax1+loadMax2���ֱ�ֵ��loadMin1��LoadMax1
            loadMax1 = loadMax1 + loadMax2;
            printf("Please enter the absolute path of unit %d characteristic data file(If the program and data files are in the same folder, you can also enter the relative path):\n", i + 1);
            scanf("%s", fileDirName2); // ������fileDirName2���洢�������ļ���·��
            if (findMinAndMaxLoad(2)) {
                return 1;
            }
            fileDirName3[4] = i + 48; // ��i���޸�����ļ����ļ�����������Ҫע��fileDirName3��char���͵ģ����ﻹ��Ҫһ��ת�����ɣ��ο�ASCII�����
            optimization();
        }
    }
}

bool findMinAndMaxLoad(int serialNum) {  // �ҵ���С������󸺺�
    lineNum = 0;
    if (serialNum == 1) { // ���ж���ţ����Ϊ1��ֱ�fileDirName1��Ӧ����С���ɺ���󸺺ɴ浽loadMin1��loadMax1��
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
    else { // ���Ϊ2��ֱ�fileDirName2��Ӧ����С���ɺ���󸺺ɴ浽LoadMin2��loadMax2��
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
        fclose(fp2); // ���������رգ���Ϊ����ռ���ڴ��
    }
    return false;
}
void optimization() { // �������ļ����������Ͻ����Ż�����������п��ܸ�������Ӧ����С��ˮ��
    fp1 = fopen(fileDirName1, "r");
    fp2 = fopen(fileDirName2, "r");
    fp3 = fopen(fileDirName3, "w");
    fprintf(fp3, "����    ��ˮ��\n");
    fprintf(fp3, "0       0\n");

    if (loadMax1 <= loadMax2 && loadMax2 <= loadMin1 + loadMin2) {
        for (load = loadMin1; load <= loadMax1 + loadMax2; load++) {
            if (load < loadMin2) { // ֻ��һ̨���ʽ�С�Ļ��鹤��
                onlyOneUnitWork(fp1, load1, waterDemand1);
                fprintf(fp3, "%-8d%s\n", load, waterDemand1);
            }
            else if (load <= loadMax1) {
                if (load < loadMax1) { // ��̨���ܽ��й�����ѡ��ˮ��С���Ǹ�
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
                if (load < loadMax2) { // ���ʽϴ��һ�����鹤��
                    onlyOneUnitWork(fp2, load2, waterDemand2);
                    fprintf(fp3, "%-8d%s\n", load, waterDemand2);
                }
                else { // load == loadMax2
                    if (loadMax2 < loadMin1 + loadMin2) { // ���ʽϴ��һ�����鹤��
                        onlyOneUnitWork(fp2, load2, waterDemand2);
                        fprintf(fp3, "%-8d%s\n", load, waterDemand2);
                    }
                    else { // loadMax2 == loadMin1 + loadMin2 ���ʽϴ��һ�������ͬ
                        onlyOneUnitWork(fp2, load2, waterDemand2);
                        float temp1 = atof(waterDemand2);
                        bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                        fprintf(fp3, "%-8d%.1f\n", load, temp1>watDemSum?watDemSum:temp1);
                    }
                    fclose(fp2);
                }
            }
            else if (load < loadMin1 + loadMin2) {
                // ɶҲ����
            }
            else { // ��̨���鹲ͬ����
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
        // ע�⣬��һ��ʽ��û��д��loadMax2==loadMax1����Ϊ��һ��if�������������
        for (load = loadMin1; load <= loadMax1 + loadMax2; load++) {
            if (load < loadMin2) { // ֻ��һ̨���ʽ�С�Ļ��鹤��
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
                if (load < loadMax1) { // ֻ��һ̨���ʽϴ�Ļ��鹤��
                    onlyOneUnitWork(fp1, load1, waterDemand1);
                    fprintf(fp3, "%-8d%s\n", load, waterDemand1);
                }
                else { // load == loadMax1
                    if (loadMax1 < loadMin1 + loadMin2) {
                        onlyOneUnitWork(fp1, load1, waterDemand1);
                        fprintf(fp3, "%-8d%s\n", load, waterDemand1);
                    }
                    else { // loadMax1 == loadMin1 + loadMin2 һ̨���ʽϴ��ͬ
                        onlyOneUnitWork(fp1, load1, waterDemand1);
                        float temp1 = atof(waterDemand1);
                        bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                        fprintf(fp3, "%-8d%.1f\n", load, temp1>watDemSum?watDemSum:temp1);
                    }
                    fclose(fp1);
                }
            }
            else if (load < loadMin1 + loadMin2) { // �м���ܳ��ֶϲ������
                // ʲôҲ���ø�
            }
            else { // ��̨���鹲ͬ�������������������loadMax1-loadMin1һ������loadMax2-loadMin2��
                bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                fprintf(fp3, "%-8d%.1f\n", load, watDemSum);
            }
        }
    }
    else if (loadMax1 <= loadMin1 + loadMin2 && loadMin1 + loadMin2 < loadMax2) {
        // ע�⣬�ڶ���ʽ��û��д��loadMin1+loadMin2<=loadMax2����Ϊ��һ��if�������������
        for (load = loadMin1; load <= loadMax1 + loadMax2; load++) {
            if (load < loadMin2) { // ֻ��һ̨���ʽ�С�Ļ��鹤��
                onlyOneUnitWork(fp1, load1, waterDemand1);
                fprintf(fp3, "%-8d%s\n", load, waterDemand1);
            }
            else if (load <= loadMax1) { // ��̨���鶼�ܽ��й���
                if (load < loadMax1) {
                    eitherOrUnitWork();
                    fprintf(fp3, "%-8d%.1f\n", load, atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1));
                }
                else { // load == loadMax1
                    if (loadMax1 < loadMin1 + loadMin2) { // ��ѡһ
                        eitherOrUnitWork();
                        fprintf(fp3, "%-8d%.1f\n", load, atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1));
                    }
                    else { // loadMax1 == loadMin1 + loadMin2 ��ѡһ��ͬ����
                        eitherOrUnitWork();
                        float temp1 = atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1);
                        bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                        fprintf(fp3, "%-8d%.1f\n", load, temp1>watDemSum?watDemSum:temp1);
                    }
                    fclose(fp1);
                }
            }
            else if (load < loadMin1 + loadMin2) { // ��������п��ܷ���
                onlyOneUnitWork(fp2, load2, waterDemand2);
                fprintf(fp3, "%-8d%s\n", load, waterDemand2);
            }
            else if (load <= loadMax2) { // ѡ����̨�����һ̨���ʽϴ�Ļ�������ͬ�����µ���С��ˮ��
                // ��������ж�loadMax1-loadMin1��loadMax2-loadMin2˭��˭С��ԭ��ο�311��
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
            else { // ��̨���鹲ͬ����
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
        // ע�⣬ʽ���еĵڶ���û��д��loadMin1+loadMin2<=loadMax1���ڶ���else if�������������
        for (load = loadMin1; load <= loadMax1 + loadMax2; load++) {
            if (load < loadMin2) { // ֻ��һ̨���ʽ�С�Ļ��鹤��
                onlyOneUnitWork(fp1, load1, waterDemand1);
                fprintf(fp3, "%-8d%s\n", load, waterDemand1);
            }
            else if (load <= loadMax2) {
                if (load < loadMax2) {
                    eitherOrUnitWork();
                    fprintf(fp3, "%-8d%.1f\n", load, atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1));
                }
                else { // load == loadMax2
                    if (loadMax2 < loadMin1 + loadMin2) { // ��ѡһ
                        eitherOrUnitWork();
                        fprintf(fp3, "%-8d%.1f\n", load, atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1));
                    }
                    else { // loadMax2 == loadMin1 + loadMin2 ��ѡһ��ͬ
                        eitherOrUnitWork();
                        float temp1 = atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1);
                        bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                        fprintf(fp3, "%-8d%.1f\n", load, temp1>watDemSum?watDemSum:temp1);
                    }
                    fclose(fp2);
                }
            }
            else if (load < loadMin1 + loadMin2) { // ֻ��һ̨���ʽϴ�Ļ��鹤��
                onlyOneUnitWork(fp1, load1, waterDemand1);
                fprintf(fp3, "%-8d%s\n", load, waterDemand1);
            }
            else if (load <= loadMax1) { // ѡ����̨�����һ̨���ʽϴ�Ļ�������ͬ�����µ���С��ˮ��
                // ����ȷ��loadMax1-loadMin1>loadMax2-loadMin2
                onlyOneUnitWork(fp1, load1, waterDemand1);
                float temp1 = atof(waterDemand1);
                bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                fprintf(fp3, "%-8d%.1f\n", load, temp1>watDemSum?watDemSum:temp1);
                if (load == loadMax1) {
                    fclose(fp1);
                }
            }
            else { // ��̨���鹤��
                bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                fprintf(fp3, "%-8d%.1f\n", load, watDemSum);
            }
        }
    }
    else if (loadMin1 + loadMin2 < loadMax1 && loadMax1 <= loadMax2) {
        // ע�⣬ʽ���еĵ�һ��û��дΪloadMin1+loadMin2<=loadMax1��������else if�������������
        for (load = loadMin1; load <= loadMax1 + loadMax2; load++) {
            if (load < loadMin2) { // ֻ��һ̨���ʽ�С�Ļ��鹤��
                onlyOneUnitWork(fp1, load1, waterDemand1);
                fprintf(fp3, "%-8d%s\n", load, waterDemand1);
            }
            else if (load < loadMin1 + loadMin2) { // ��ѡһ
                eitherOrUnitWork();
                fprintf(fp3, "%-8d%.1f\n", load, atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1));
            }
            else if (load <= loadMax1) { // ��̨������Ե��ɣ�Ҳ���Թ�ͬ��
                eitherOrUnitWork();
                float temp1 = atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1);
                // ����˵��һ�£�ΪʲôbothUnitsWorkû���ж�loadMax1-loadMin1��loadMax2-loadMin2˭��˭С���������Χ�ڣ�load���ȡֵΪloadMax1����lineNum=load-loadMin1-loadMin2<=loadMax1-loadMin1-loadMin2����loadMax1-loadMin1-loadMin2��С��loadMax1-loadMin1��ҲС��loadMax2-loadMin2��ɶ��˼�أ�lineNumһ��С��smallAmbit�������˵����lineNum�ķ��䲻��Խ�磬���������䡣
                bothUnitsWork(loadMax1-loadMin1, fileDirName1, loadMax2-loadMin2, fileDirName2);
                fprintf(fp3, "%-8d%.1f\n", load, temp1>watDemSum?watDemSum:temp1);
                if (load == loadMax1) {
                    fclose(fp1);
                    if (loadMax1 == loadMax2) {
                        fclose(fp2);
                    }
                }
            }
            else if (load <= loadMax2) { // ���ʽϴ�Ļ��鵥�ɻ�ͬ��
                onlyOneUnitWork(fp2, load2, waterDemand2);
                float temp1 = atof(waterDemand2);
                // �ο�311�У������û�а취��֤lineNum����Խ��
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
            else { // ��ͬ��
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
        // ���ǰ�ÿ�����û���ظ��ؽ����˷��࣬���в��
        for (load = loadMin1; load <= loadMax1 + loadMax2; load++) {
            if (load < loadMin2) { // ֻ��һ̨���ʽ�С�Ļ��鹤��
                onlyOneUnitWork(fp1, load1, waterDemand1);
                fprintf(fp3, "%-8d%s\n", load, waterDemand1);
            }
            else if (load < loadMin1 + loadMin2) { // ��ѡһ
                eitherOrUnitWork();
                fprintf(fp3, "%-8d%.1f\n", load, atof(waterDemand1)>atof(waterDemand2)?atof(waterDemand2):atof(waterDemand1));
            }
            else if (load <= loadMax2) { // ��̨������Ե��ɣ�Ҳ���Թ�ͬ��
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
void onlyOneUnitWork(FILE *fp, char *charLoad, char *waterDemand) { // ֻ��һ̨�����ܹ���
    while (fscanf(fp, "%s%s", charLoad, waterDemand) != EOF) {
        if (load == atoi(charLoad)) {
            break;
        }
    }
}
void eitherOrUnitWork() { // ��̨���鶼�ܽ��й�������ѡ����ͬ��������ˮ��С�Ļ�����й���
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
void bothUnitsWork(int greatAmbit, char *fileDirName1, int smallAmbit, char *fileDirName2) { // ��̨���鹲ͬ����
    lineNum = load - loadMin1 - loadMin2;
    if (lineNum <= smallAmbit) {
        for (int i = 0; i <= lineNum; i++) { // ��Ȧ�����Ƿ����
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