#include <iostream>
#include <cstring>
#include <queue>
#include <vector>
#include <algorithm>
#include <Windows.h>

using namespace std;

struct Node{
	int m;	
	int c;
	int b;
	int step;
	double f_loss;
	Node* parent;
	Node(int m_num, int c_num, int boat_state, int step_count, Node* ptr): m{m_num}, c{c_num}, b{boat_state}, step{step_count}, parent{ptr} {
		f_loss = (m + c) / 2;
	}
	Node():m{0}, c{0}, b{0}, step{0}, f_loss{0}, parent{nullptr} {};
};

deque<Node> opened_list;
vector<int> closed_list;
int m_num, c_num;

bool is_safe(Node* n){	// 判斷兩岸的傳教士是否安全
	int m_left = n->m;
	int c_left = n->m;
	int m_right =  m_num - m_left;
	int c_right = c_num - c_left;

	if(m_left<c_left && m_left!=0)	// 左岸食人魔數量不可超過傳教士數量，除非傳教士數量為0
		return false;
	if(m_right<c_right && m_right!=0)	// 右岸食人魔數量不可超過傳教士數量，除非傳教士數量為0
		return false;
	if(m_left<0 || c_left<0)
		return false;
	if(m_left>=c_left || m_right>=c_right)
		return true;
}

int cal_value(Node* node){
	return node->m*100 + node->c*10 + node->b;
}

bool in_closed_list(Node* node){	// 確認是否已經在closed_list內
	int value = cal_value(node);
	if(find(closed_list.begin(), closed_list.end(), value)!=closed_list.end())
		return true;
	return false;
}

void sort_by_floss(){	// 將opened_list內的點按照分數大小排序
	for(deque<Node>::iterator i=opened_list.begin(); i!=opened_list.end(); i++){
		for(deque<Node>::iterator j=i+1; j!=opened_list.end(); j++){
			if(i->f_loss>j->f_loss){
				swap(i, j);
			}
			else if(i->step>j->step){
				swap(i, j);
			}
		}
	}
}

void swap(Node &a, Node &b){
	Node tmp;
	tmp = a;
	a = b;
	b = tmp;
}

void refresh_opened(Node* n){	// 如果有較低的分數，更新opened_list
	for(deque<Node>::iterator it=opened_list.begin(); it!=opened_list.end();it++){
		if(it->m==n->m && it->c==n->c && it->b==n->b){
			if(n->f_loss<it->f_loss){
				memcpy(&it, n, sizeof(Node));
			}
			return;
		}
	}
	opened_list.push_back(Node(n->m, n->c, n->b, n->step, n->parent));
}

void a_star_algorithm(Node& result){
	while(opened_list.size() != 0){
		// 從opened_list中取出分數最小的
		Node node;
		node = opened_list.front();
		opened_list.pop_front();
		// 判斷取出的點是否為目標點
		if(node.m == 0 && node.c == 0 && node.b == 0){
			result = node;
			break;
		}
		// 將取出的點加入closed_list中
		closed_list.push_back(cal_value(&node));
		for(int i=0; i<=m_num; i++){		// i代表上船的傳教士數量
			for(int j=0; j<=c_num; j++){	// j代表上船的食人魔數量
				// 判斷船上的合理情況
				if(i+j==0 || i+j>2 || (i!=0 && i<j)){
					continue;
				}
				if(node.b == 1){	// 船在左側，下一狀態船在右側
					Node* child_node = new Node(node.m - i, node.c - j, 0, node.step + 1, &node);
					if(!in_closed_list(child_node)){
						if(is_safe(child_node)){
							refresh_opened(child_node);
						}
					}
				}
				else{	// 船在右側，下一狀態船在左側
					Node* child_node = new Node(node.m + i, node.c + j, 1, node.step + 1, &node);
					if(!in_closed_list(child_node)){
						if(is_safe(child_node)){
							refresh_opened(child_node);
						}
					}
				}
			}
		}
		sort_by_floss();
	}
}

int main(int argc, char const *argv[])
{
	SetConsoleOutputCP(CP_UTF8);

	cout << "請輸入初始傳教士人數：";
	cin >> m_num;
	cout << "請輸入初始野人人數：";
	cin >> c_num;

	Node start_node(m_num, c_num, 1, 0, nullptr);
	opened_list.push_back(start_node);

	Node result;
	a_star_algorithm(result);

	cout << result.step << endl;

	return 0;
}