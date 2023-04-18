#include<stdio.h>

struct unit{ // 通过结构体把三台机组的特性资料存储起来
    int loadMin, loadMax;
    float watDem[8]; // water demand，需水量
};
struct unit unit1 = {5, 12, {251, 297.9, 344.5, 393.1, 445.8, 500.5, 555, 610.4}};
struct unit unit2 = {7, 14, {346.7, 395.3, 444.7, 493.9, 545, 598.1, 652, 709.1}};
struct unit unit3 = {10, 17, {508.2, 561.7, 616, 673.5, 733, 793.5, 856.1, 920.8}};

float watDemMin1[22]; // watDemMin1[22]把机组1和机组2所有组合负荷对应的最小需水量存储起来
float watDemMin2[39]; // watDemMin2[39]把机组1和机组2这个整体与机组3所有组合负荷对应的最小需水量存储起来

float bothUnitsWork1(int tempLoad); // 机组1和机组2共同工作
float bothUnitsWork2(int tempLoad); // 机组1和机组2工作的最优化结果再和机组3共同工作

int main() {
    int tempLoad, distributedLoad; // tempLoad则用于遍历所有的负荷，distributedLoad则用于两台机组共同工作时负荷的分配调节，具体的调节原理见下面第二个for循环中else注释。
    
    // 第一个阶段，机组1和机组2进行配合
    tempLoad = unit1.loadMin;
    for (; tempLoad < unit2.loadMin; tempLoad++) {
        watDemMin1[tempLoad-unit1.loadMin] = unit1.watDem[tempLoad-unit1.loadMin];
        // tempLoad-unit1.loadMin可以表示watDemMin1[]和unit1.watDem[]的下标（用负荷巧妙建立与数组下标的关系，从而可以省略一个变量来表示数组下标）
    }
    for (; tempLoad <= unit1.loadMax; tempLoad++) {
        if (tempLoad < unit1.loadMax) {
            watDemMin1[tempLoad-unit1.loadMin] = unit1.watDem[tempLoad-unit1.loadMin]>unit2.watDem[tempLoad-unit2.loadMin]?unit2.watDem[tempLoad-unit2.loadMin]:unit1.watDem[tempLoad-unit1.loadMin];
            // tempLoad-unit2.loadMin可以表示unit2.watDem[]的下标
        }
        else { // tempLoad == unit1.loadMax
            float temp1 = unit1.watDem[tempLoad-unit1.loadMin]>unit2.watDem[tempLoad-unit2.loadMin]?unit2.watDem[tempLoad-unit2.loadMin]:unit1.watDem[tempLoad-unit1.loadMin];
            // 此时tempLoad遍历到了unit1.loadMax，同时也等于unit1.loadMin+unit2.loadMin，这意味着两台机组共同工作也可以满足负荷需求了，但是机组间的负荷如何分配呢？让distributedLoad = tempLoad - unit1.loadMin - unit2.loadMin，如果在已经分别分配给unit1机组、unit2机组unit1.loadMin、unit2.loadMin个负荷的情况下分配给unit1机组i个负荷，那么就不得不分配给unit2机组distributedLoad-i个负荷（这句话是关键，没有读懂再读两遍）。
            // 举两个例子：当tempLoad等于12的时候，distributedLoad=12-5-7=0，也就是说这时候没有“多余”的负荷需要分配，只需要给unit1机组分配unit1.loadMin个负荷，给unit2机组分配unit2.loadMin个负荷即可；当tempLoad等于13的时候，distributedLoad=13-5-7=1，这个时候有“多余”的负荷了，在已经分别给unit1、unit2分配unit1.loadMin、unit2.loadMin的前提下，既可以将这1个负荷分给unit1，也可以将这1个负荷分给unit2。与直接给两台机组分配负荷不同的是，这种方法先给两台机组分配两台机组对应的最小符合，然后将“多余”的负荷进行分配。
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

    // 第二个阶段，watDemMin1里面的数据和机组3里面的数据进行比较
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

    // 将负荷和最小需水量打印出来
    for (int i = unit1.loadMin; i <= unit1.loadMax + unit2.loadMax + unit3.loadMax; i++) {
        printf("%d\t%.1f\n", i, watDemMin2[i-unit1.loadMin]);
    }
}
float bothUnitsWork1(int tempLoad) { // 将tempLoad对应的两机组共同工作的最小需水量返回
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
        for (int i = distributedLoad-7; i <= 7; i++) { // 这种方法有点别出心裁
            if (i == distributedLoad-7) { // 注意这里是从distributedLoad-7开始的
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