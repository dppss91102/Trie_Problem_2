#include <iostream>
#include <vector>
#include <map>
#include <list>

using namespace std;
#define maxCharNum 256 //定義一行最多幾個char

class suffix_trie {
public:
    suffix_trie(){
        nextNode = 0;
        temp = 0;
        matching = false;
    }
    void makeTrie(char* cursor, int currentNode){
        /* cursor   -> 指向字元的指標
         * currentNode  -> vector的index(現在的node)
         * nextNode     -> 下一個要指到的node
         * */

        //recursive的終止條件，讀到空字元 = 字串結束
        if(*cursor == '\0')
            return;

        //如果pattern[currentNode]不存在，則放入空的map直到pattern[currentNode]
        while (patterns.size() <= currentNode + 1){
            map<int, char> map1;
            patterns.push_back(map1);
        }

        //如果這個node底下還沒有分支
        if (patterns[currentNode].empty()) {
            //創一個分支指到nextNode
            patterns[currentNode].insert(pair<int, char>(++nextNode, *cursor));
            //cout << currentNode << "->" << nextNode << ":" << patterns[currentNode].at(nextNode) << endl;//for debugging
            //下一個字元，接在上面的node之後
            makeTrie(cursor+1, currentNode+1);
            return;
        }

        //如果這個node底下有分支了，檢查分支裡面是否有現在的字元
        for (auto &it : patterns[currentNode]) {
            if (it.second == *cursor){
                //如果有的話，則下一個字元接在它的node之後
                makeTrie(cursor+1, it.first);
                return;
            }
        }

        //會到這邊則代表這個node底下有分支且都不是現在的字元
        //直接創一個分支指到nextNode
        patterns[currentNode].insert(std::pair<int, char>(++nextNode, *cursor));
        //cout << currentNode << "->" << nextNode << ":" << patterns[currentNode].at(nextNode) << endl;//for debugging
        makeTrie(cursor+1, nextNode);
    }
    void matchText(char* p, int cursor, int currentNode){
        //讀到字串結束
        if(p[cursor] == '\0')
            return;

        //空的則代表此node為leaf node
        if (patterns[currentNode].empty()) {
            matching = false;
            matchText(p, temp, 0);
            return;
        }

        //如果這個node底下有分支了，檢查分支裡面是否有現在的字元
        for (auto &it : patterns[currentNode]) {
            if (it.second == p[cursor]){
                if (currentNode == 0) {
                    pos.push_back(cursor);
                    temp = cursor + 1;//將下一個字元的位置暫存起來
                    matching = true;
                    matchText(p, cursor + 1, it.first);
                    return;
                }
                matchText(p, cursor + 1, it.first);
                return;
            }
        }

        //如果正在matching，代表match失敗，把錯誤結果pop掉，回到暫存字元處。回到root。
        if (matching) {
            pos.pop_back();
            matching = false;
            matchText(p, temp, 0);
        } else //沒在matching，直接前往下個字元。回到root。
            matchText(p, cursor+1, 0);
    }
    void print(){
        while (!pos.empty()){
            if (pos.front() == pos.back())
                cout << pos.front();
            else
                cout << pos.front() << ' ';

            pos.pop_front();
        }
    }
private:
    vector<map<int, char>> patterns;
    list<int> pos;//儲存有matched的起始位置
    int nextNode;//node的編號
    int temp;//暫存位置
    bool matching;//是否正在matching
};


int main() {

    char text[maxCharNum];
    cin.getline(text, maxCharNum);
    int n;
    cin >> n;
    cin.ignore();//沒有這個的話getline會吃到\n

    suffix_trie trie;
    while (n-->0){
        //把一整行讀進p，最多maxCharNum個字元，包含最後的'\0'
        char p[maxCharNum];
        cin.getline(p, maxCharNum);

        trie.makeTrie(p, 0);
    }

    trie.matchText(text, 0, 0);

    trie.print();

    return 0;
}





