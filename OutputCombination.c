#include<stdio.h>

struct unit{ // ͨ���ṹ�����̨������������ϴ洢����
    int loadMin, loadMax;
    float watDem[8]; // water demand����ˮ��
};
struct unit unit1 = {5, 12, {251, 297.9, 344.5, 393.1, 445.8, 500.5, 555, 610.4}};
struct unit unit2 = {7, 14, {346.7, 395.3, 444.7, 493.9, 545, 598.1, 652, 709.1}};
struct unit unit3 = {10, 17, {508.2, 561.7, 616, 673.5, 733, 793.5, 856.1, 920.8}};

float watDemMin1[22]; // watDemMin1[22]�ѻ���1�ͻ���2������ϸ��ɶ�Ӧ����С��ˮ���洢����
float watDemMin2[39]; // watDemMin2[39]�ѻ���1�ͻ���2������������3������ϸ��ɶ�Ӧ����С��ˮ���洢����

float bothUnitsWork1(int tempLoad); // ����1�ͻ���2��ͬ����
float bothUnitsWork2(int tempLoad); // ����1�ͻ���2���������Ż�����ٺͻ���3��ͬ����

int main() {
    int tempLoad, distributedLoad; // tempLoad�����ڱ������еĸ��ɣ�distributedLoad��������̨���鹲ͬ����ʱ���ɵķ�����ڣ�����ĵ���ԭ�������ڶ���forѭ����elseע�͡�
    
    // ��һ���׶Σ�����1�ͻ���2�������
    tempLoad = unit1.loadMin;
    for (; tempLoad < unit2.loadMin; tempLoad++) {
        watDemMin1[tempLoad-unit1.loadMin] = unit1.watDem[tempLoad-unit1.loadMin];
        // tempLoad-unit1.loadMin���Ա�ʾwatDemMin1[]��unit1.watDem[]���±꣨�ø���������������±�Ĺ�ϵ���Ӷ�����ʡ��һ����������ʾ�����±꣩
    }
    for (; tempLoad <= unit1.loadMax; tempLoad++) {
        if (tempLoad < unit1.loadMax) {
            watDemMin1[tempLoad-unit1.loadMin] = unit1.watDem[tempLoad-unit1.loadMin]>unit2.watDem[tempLoad-unit2.loadMin]?unit2.watDem[tempLoad-unit2.loadMin]:unit1.watDem[tempLoad-unit1.loadMin];
            // tempLoad-unit2.loadMin���Ա�ʾunit2.watDem[]���±�
        }
        else { // tempLoad == unit1.loadMax
            float temp1 = unit1.watDem[tempLoad-unit1.loadMin]>unit2.watDem[tempLoad-unit2.loadMin]?unit2.watDem[tempLoad-unit2.loadMin]:unit1.watDem[tempLoad-unit1.loadMin];
            // ��ʱtempLoad��������unit1.loadMax��ͬʱҲ����unit1.loadMin+unit2.loadMin������ζ����̨���鹲ͬ����Ҳ�������㸺�������ˣ����ǻ����ĸ�����η����أ���distributedLoad = tempLoad - unit1.loadMin - unit2.loadMin��������Ѿ��ֱ�����unit1���顢unit2����unit1.loadMin��unit2.loadMin�����ɵ�����·����unit1����i�����ɣ���ô�Ͳ��ò������unit2����distributedLoad-i�����ɣ���仰�ǹؼ���û�ж����ٶ����飩��
            // ���������ӣ���tempLoad����12��ʱ��distributedLoad=12-5-7=0��Ҳ����˵��ʱ��û�С����ࡱ�ĸ�����Ҫ���䣬ֻ��Ҫ��unit1�������unit1.loadMin�����ɣ���unit2�������unit2.loadMin�����ɼ��ɣ���tempLoad����13��ʱ��distributedLoad=13-5-7=1�����ʱ���С����ࡱ�ĸ����ˣ����Ѿ��ֱ��unit1��unit2����unit1.loadMin��unit2.loadMin��ǰ���£��ȿ��Խ���1�����ɷָ�unit1��Ҳ���Խ���1�����ɷָ�unit2����ֱ�Ӹ���̨������为�ɲ�ͬ���ǣ����ַ����ȸ���̨���������̨�����Ӧ����С���ϣ�Ȼ�󽫡����ࡱ�ĸ��ɽ��з��䡣
            float temp2 = bothUnitsWork1(tempLoad);
            watDemMin1[tempLoad-unit1.loadMin] = temp1>temp2?temp2:temp1;
        }
    }
    for (; tempLoad <= unit2.loadMax; tempLoad++) {
        float temp1 = bothUnitsWork1(tempLoad);
        watDemMin1[tempLoad-unit1.loadMin] = temp1>unit2.watDem[tempLoad-unit2.loadMin]?unit2.watDem[tempLoad-unit2.loadMin]:temp1;
    }
    for (; tempLoad <= unit1.loadMax + unit2.loadMax; tempLoad++) {
        watDemMin1[tempLoad-unit1.loadMin] = bothUnitsWork1(tempLoad);
    }

    // �ڶ����׶Σ�watDemMin1��������ݺͻ���3��������ݽ��бȽ�
    tempLoad = unit1.loadMin;
    for (; tempLoad < unit3.loadMin; tempLoad++) {
        watDemMin2[tempLoad-unit1.loadMin] = watDemMin1[tempLoad-unit1.loadMin];
    }
    for (; tempLoad < unit1.loadMin + unit3.loadMin; tempLoad++) {
        watDemMin2[tempLoad-unit1.loadMin] = watDemMin1[tempLoad-unit1.loadMin]>unit3.watDem[tempLoad-unit3.loadMin]?unit3.watDem[tempLoad-unit3.loadMin]:watDemMin1[tempLoad-unit1.loadMin];
    }
    for (; tempLoad <= unit3.loadMax; tempLoad++) {
        float temp1, temp2;
        temp1 = watDemMin1[tempLoad-unit1.loadMin]>unit3.watDem[tempLoad-unit3.loadMin]?unit3.watDem[tempLoad-unit3.loadMin]:watDemMin1[tempLoad-unit1.loadMin];
        temp2 = bothUnitsWork2(tempLoad);
        watDemMin2[tempLoad-unit1.loadMin] = temp1>temp2?temp2:temp1;
    }
    for (; tempLoad <= unit1.loadMax + unit2.loadMax; tempLoad++) {
        float temp1 = bothUnitsWork2(tempLoad);
        watDemMin2[tempLoad-unit1.loadMin] = temp1>watDemMin1[tempLoad-unit1.loadMin]?watDemMin1[tempLoad-unit1.loadMin]:temp1;
    }
    for (; tempLoad <= unit1.loadMax + unit2.loadMax + unit3.loadMax; tempLoad++) {
        watDemMin2[tempLoad-unit1.loadMin] = bothUnitsWork2(tempLoad);
    }

    // �����ɺ���С��ˮ����ӡ����
    for (int i = unit1.loadMin; i <= unit1.loadMax + unit2.loadMax + unit3.loadMax; i++) {
        printf("%d\t%.1f\n", i, watDemMin2[i-unit1.loadMin]);
    }
}
float bothUnitsWork1(int tempLoad) { // ��tempLoad��Ӧ�������鹲ͬ��������С��ˮ������
    float watDemSumMin;
    int distributedLoad = tempLoad - unit1.loadMin - unit2.loadMin;
    if (distributedLoad <= 7) {
        for (int i = 0; i <= distributedLoad; i++) {
            if (i == 0) {
                watDemSumMin = unit1.watDem[i] + unit2.watDem[distributedLoad-i];
            }
            else {
                if (watDemSumMin > unit1.watDem[i] + unit2.watDem[distributedLoad-i]) {
                    watDemSumMin = unit1.watDem[i] + unit2.watDem[distributedLoad-i];
                }
            }
        }
    }
    else { // distributedLoad > 7
        for (int i = distributedLoad-7; i <= 7; i++) { // ���ַ����е����Ĳ�
            if (i == distributedLoad-7) { // ע�������Ǵ�distributedLoad-7��ʼ��
                watDemSumMin = unit1.watDem[i] + unit2.watDem[distributedLoad-i];
            }
            else {
                if (watDemSumMin > unit1.watDem[i] + unit2.watDem[distributedLoad-i]) {
                    watDemSumMin = unit1.watDem[i] + unit2.watDem[distributedLoad-i];
                }
            }
        }
    }
    return watDemSumMin;
}
float bothUnitsWork2(int tempLoad) {
    float watDemSumMin;
    int distributedLoad = tempLoad - unit1.loadMin - unit3.loadMin;
    if (distributedLoad <= 7) {
        for (int i = 0; i <= distributedLoad; i++) {
            if (i == 0) {
                watDemSumMin = watDemMin1[i] + unit3.watDem[distributedLoad-i];
            }
            else {
                if (watDemSumMin > watDemMin1[i] + unit3.watDem[distributedLoad-i]) {
                    watDemSumMin = watDemMin1[i] + unit3.watDem[distributedLoad-i];
                }
            }
        }
    }
    else if (distributedLoad <= 21) {
        for (int i = 0; i <= 7; i++) {
            if (i == 0) {
                watDemSumMin = watDemMin1[distributedLoad-i] + unit3.watDem[i];
            }
            else {
                if (watDemSumMin > watDemMin1[distributedLoad-i] + unit3.watDem[i]) {
                    watDemSumMin = watDemMin1[distributedLoad-i] + unit3.watDem[i];
                }
            }
        }
    }
    else {
        for (int i = 0; i <= 28 - distributedLoad; i++) {
            if (i == 0) {
                watDemSumMin = watDemMin1[distributedLoad-7+i] + unit3.watDem[7-i];
            }
            else {
                if (watDemSumMin > watDemMin1[distributedLoad-7+i] + unit3.watDem[7-i]) {
                    watDemSumMin = watDemMin1[distributedLoad-7+i] + unit3.watDem[7-i];
                }
            }
        }
    }
    return watDemSumMin;
}