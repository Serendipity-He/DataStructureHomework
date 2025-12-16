#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <climits>
#include <string>
using namespace std;

typedef struct {
    char ch;       // 叶子节点对应字符，非叶子节点为'#'
    int weight;    // 节点权重（字符频率）
    int parent;    // 父节点下标（0表示无父节点）
    int lchild;    // 左孩子下标
    int rchild;    // 右孩子下标
} HTNode, *HuffmanTree;

void SelectMin(HuffmanTree HT, int end, int &s1, int &s2) {
    int minVal = INT_MAX;
    // 找第一个最小节点
    for (int i = 1; i <= end; ++i) {
        if (HT[i].parent == 0 && HT[i].weight < minVal) {
            minVal = HT[i].weight;
            s1 = i;
        }
    }
    // 找第二个最小节点（排除s1）
    minVal = INT_MAX;
    for (int i = 1; i <= end; ++i) {
        if (HT[i].parent == 0 && i != s1 && HT[i].weight < minVal) {
            minVal = HT[i].weight;
            s2 = i;
        }
    }
    // 确保s1权重不大于s2
    if (HT[s1].weight > HT[s2].weight) swap(s1, s2);
}

void CreateHuffmanTree(HuffmanTree &HT, const vector<pair<char, int>> &charsFreq, int k) {
    if (k <= 1) return;
    int m = 2 * k - 1; // 哈夫曼树总节点数
    HT = new HTNode[m + 1]; // 下标1~m（0位空置）

   
    for (int i = 1; i <= k; ++i) {
        HT[i].ch = charsFreq[i-1].first;
        HT[i].weight = charsFreq[i-1].second;
        HT[i].parent =0;
        HT[i].lchild =0;
        HT[i].rchild = 0;
    }   

    for (int i = k+1; i <= m; ++i) {
        HT[i].ch = '#';
        HT[i].weight =0;
        HT[i].parent =0;
        HT[i].lchild =0;
        HT[i].rchild = 0;
    }

    for (int i = k+1; i <= m; ++i) {
        int s1, s2;
        SelectMin(HT, i-1, s1, s2); // 选前i-1个节点中最小的两个
        HT[s1].parent = HT[s2].parent = i;
        HT[i].lchild = s1;
        HT[i].rchild = s2;
        HT[i].weight = HT[s1].weight + HT[s2].weight;
    }
}

// 生成哈夫曼编码表（字符→编码）
void CreateHuffmanCode(HuffmanTree HT, int k, map<char, string> &codeMap) {
    for (int i = 1; i <= k; ++i) {
        string code;
        int curr = i;
        int parent = HT[curr].parent;
        // 从叶子节点逆向遍历到根节点，生成编码
        while (parent != 0) {
            if (HT[parent].lchild == curr) {
                code = "0" + code;
            }
            else code = "1" + code;
            curr = parent;
            parent = HT[curr].parent;
        }
        codeMap[HT[i].ch] = code;
    }
}

// 编码原字符串
string encodeString(const string &originStr, const map<char, string> &codeMap) {
    string encodeStr;
    for (char c : originStr) encodeStr += codeMap.at(c);
    return encodeStr;
}

// string decodeString(const string &encodeStr, HuffmanTree HT, int m,bool flag) {
//     string decodeStr;
//     int curr = m; // 从根节点开始遍历
//     for (char bit : encodeStr) {
//         // 按0→左孩子、1→右孩子遍历
//         curr = (bit == '0') ? HT[curr].lchild : HT[curr].rchild;
//         if (HT[curr].lchild == 0 && HT[curr].rchild == 0) {
//             decodeStr += HT[curr].ch;
//             curr = m;
//         }
//     }
//     if (HT[curr].ch == '#'&&curr!=m)
//     {
//         flag=false;
//     }
    
//     return decodeStr;
// }
string decodeString(const string &encodeStr, HuffmanTree HT, int m, bool &flag) {
    string decodeStr;
    int curr = m; // 从根节点开始遍历
    flag = true;  // 初始化为解码成功

    if (m == 1) {
        decodeStr.append(encodeStr.size(), HT[1].ch);
        return decodeStr;
    }

    for (char bit : encodeStr) {
        if (bit == '0') {
            if (HT[curr].lchild == 0) {
                flag = false;
                cerr << "解码失败：节点" << curr << "无左孩子，无法处理位'0'！" << endl;
                return "";
            }
            curr = HT[curr].lchild;
        } else if (bit == '1') {
            if (HT[curr].rchild == 0) {
                flag = false;
                cerr << "解码失败：节点" << curr << "无右孩子，无法处理位'1'！" << endl;
                return "";
            }
            curr = HT[curr].rchild;
        }
        if (HT[curr].lchild == 0 && HT[curr].rchild == 0) {
            decodeStr += HT[curr].ch;
            curr = m;
        }
    }
    if (curr != m) {
        flag = false;
        cerr << "解码失败：输入的编码序列不完整，最后部分无法解码！" << endl;
    }

    return decodeStr;
}

// 检测解码输入的合法性：空序列、非0/1字符
bool checkDecodeInput(const string &inputStr) {
    // 检测空序列
    if (inputStr.empty()) {
        cerr << "错误：待解码的序列为空！" << endl;
        return false;
    }

    // 检测非0/1字符
    for (char c : inputStr) {
        if (c != '0' && c != '1') {
            cerr << "错误：待解码序列包含非法字符'" << c << "'（仅支持0和1）！" << endl;
            return false;
        }
    }

    return true;
}

bool checkInput(const string &inputStr) {
    for (char c : inputStr) {
        if (c != '0' && c != '1') {
            cout << "输入序列包含非01字符" << endl;
            return false;
        }
    }
    return true;
}
int main() {
    cout << "请输入待编码的字符串:" << endl;
    string originStr;
    getline(cin, originStr);

    vector<int> freq(26, 0);
    for (char c : originStr) {
        if (c < 'A' || c > 'Z') {
            cout << "字符串包含非大写字母字符" << endl;
            return 1;
        }
        freq[c - 'A']++;
    }
    vector<pair<char, int>> charsFreq;
    for (int i = 0; i < 26; ++i) {
        if (freq[i] > 0) charsFreq.emplace_back('A' + i, freq[i]);
    }
    int k = charsFreq.size();
    if (k == 0) {
        cout << "字符串为空" << endl;
        return 1;
    }

     
    HuffmanTree HT = nullptr;
    CreateHuffmanTree(HT, charsFreq, k);
    int m = 2 * k - 1;

     
    map<char, string> codeMap;
    CreateHuffmanCode(HT, k, codeMap);
    cout << "\n哈夫曼编码表:" << endl;
    for (const auto &p : codeMap){
    cout << p.first << ":" << p.second << endl;    
    } 

    string encodeStr = encodeString(originStr, codeMap);
    cout << "\n编码后的序列:" << encodeStr << endl;
    
    string inputEncode;
    cout << "\n请输入待解码的01序列:" << endl;
    getline(cin, inputEncode);
    bool flag=true;
    string decodeStr = decodeString(inputEncode, HT, m,flag);
    if(flag==false)
    {
        cout<<"解码失败"<<endl;
        return 1;
    }
    cout << "解码后的原字符串:" << decodeStr << endl;

    delete[] HT;
    return 0;
}