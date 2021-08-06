/*
 * @Author: your name
 * @Date: 2021-08-05 18:09:06
 * @LastEditTime: 2021-08-06 08:40:45
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \1.Firmware\include\ring_queue.h
 */

//
// Created by WangZX on 2021/8/5.
//

#ifndef CLION_RING_QUEUE_H
#define CLION_RING_QUEUE_H

#include <vector>
using namespace std;

template <class T>
class ring_queue
{
private:
    vector<T> buffer;
    int bufSize;
    int front;
    int rear;
    
public:
    bool isFull();
    bool isEmpty();
    ring_queue(int n);
    ~ring_queue();
    void push(T x);
    void pop();
    T getFront();
    T getBack();
    int size();
    int bufferSize();
};

template <class T>
ring_queue<T>::ring_queue(int n)
{
    buffer.resize(n);
    bufSize = n;
    front = 0;
    rear = 0;
}
template <class T>
ring_queue<T>::~ring_queue()
{
}

template <class T>
void ring_queue<T>::push(T x) {
    if (this->isFull()) {
        return;
    }
    buffer[this->rear] = x;
    this->rear = (this->rear + 1) % this->bufSize;
}

template <class T>
void ring_queue<T>::pop() {
    if (this->isEmpty()) {
        return;
    }
    this->front = (this->front + 1) % this->bufSize;
}

template <class T>
T ring_queue<T>::getFront() {
    return buffer[front];
}

template <class T>
T ring_queue<T>::getBack() {
    return buffer[(rear  + buffersize - 1) % bufferSize];
}

template <class T>
int ring_queue<T>::size() {
    return this->rear - this->front - 1;
}

template <class T>
int ring_queue<T>::bufferSize() {
    return this->bufSize;
}

template <class T>
bool ring_queue<T>::isFull() {
    return (this->rear + 1) % this->bufSize == this->front;
}

template<class T>
bool ring_queue<T>::isEmpty() {
    return this->rear == this->front;
}

#endif //CLION_RING_QUEUE_H


