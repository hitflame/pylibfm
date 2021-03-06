//
// Created by yanyu on 2017/7/4.
// 寻找特征矩阵的SGD算法
//
#include "model.h"

#ifndef PYLIBFM_SGD_H
#define PYLIBFM_SGD_H

/* 用随机梯度下降计算出Factorization Machines的矩阵权重
 * fmModel:    训练的模型参数
 * learn_rate: 学习率
 * x:          当前训练行(随机梯度下降法是训练完一行调整一次模型的w,而不是全部训练完再去改)
 * multiplier: 训练结果和真实值之间的误差
 * sum:        当前vx的各维度的累加
 */
void fm_SGD(FMModel* fm, const double& learn_rate, SparseRow<float> &x, const double multiplier, DVector<double> &sum){
    if (fm->is_use_w0) {
        fm->w0 -= learn_rate * (multiplier + fm->reg0 * fm->w0);
    }
    if (fm->is_use_w) {
        for (uint i = 0; i < x.size; i++) {
            double& w = fm->w(x.data[i].id);
            w -= learn_rate * (multiplier * x.data[i].value + fm->regw * w);
        }
    }
    for(int f = 0; f < fm->num_factor; f++){
        for (uint i = 0; i < x.size; i++) {
            double& v = fm->v(f,x.data[i].id);
            double grad = sum(f) * x.data[i].value - v * x.data[i].value * x.data[i].value;
            v -= learn_rate * (multiplier * grad + fm->regv * v);
        }
    }
}

#endif //PYLIBFM_SGD_H
