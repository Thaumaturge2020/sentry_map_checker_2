/*************************************************************************
	> File Name: main.cpp
	> Author: The-Out-Land
	> Mail: 2264454706@qq.com 
	> Created Time: Tue 21 May 2024 08:39:25 PM CST
 ************************************************************************/

#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <toml.hpp>

#define enter putchar('\n')
#define space putchar(' ')
#define re register
#define pii std::pair<int,int>

using namespace std;

inline int read(){
	int x=0;char c=getchar();bool y=1;
	for(;c<'0' || c>'9';c=getchar()) if(c=='-') y=0;
	for(;c>='0' && c<='9';c=getchar()) x=(x<<1)+(x<<3)+c-48;
	if(y) return x;
	return -x;
}

class PointChooser{
	public:
	cv::Mat map_img;
	int height;
	int width;
	int mode;
	int choose_id;
	int total_point_num;
	std::vector<int> empty;
	std::map<int,std::pair<int,int> > point_set;
	std::map<int,std::vector< int > > point_map;
	double save_rate_x;
	double save_rate_y;

	double expected_height;
	double expected_width;

	PointChooser(){
		mode = 0;
		choose_id = 0;
		empty.clear();
		save_rate_x = save_rate_y = 1;
		total_point_num = 0;
	}

	std::vector<cv::Scalar> color_array;

	void set_hw(double height,double width){
		expected_height = height;
		expected_width = width;
	}

	void color_init(){
		color_array.push_back(cv::Scalar(0,0,255));
		color_array.push_back(cv::Scalar(0,255,0));
		color_array.push_back(cv::Scalar(255,0,0));
		color_array.push_back(cv::Scalar(0,255,255));
		color_array.push_back(cv::Scalar(255,0,255));
		color_array.push_back(cv::Scalar(255,255,0));
		color_array.push_back(cv::Scalar(128,128,128));
		color_array.push_back(cv::Scalar(128,0,0));
		color_array.push_back(cv::Scalar(128,128,0));
		color_array.push_back(cv::Scalar(0,128,0));
		color_array.push_back(cv::Scalar(128,0,128));
		color_array.push_back(cv::Scalar(0,128,128));
		color_array.push_back(cv::Scalar(0,0,128));
		color_array.push_back(cv::Scalar(255,165,0));
		color_array.push_back(cv::Scalar(255,0,165));
		color_array.push_back(cv::Scalar(165,255,0));
		color_array.push_back(cv::Scalar(0,255,165));
		color_array.push_back(cv::Scalar(165,0,255));
		color_array.push_back(cv::Scalar(0,165,255));
		color_array.push_back(cv::Scalar(255,255,165));
		color_array.push_back(cv::Scalar(255,165,255));
		color_array.push_back(cv::Scalar(165,255,255));
		color_array.push_back(cv::Scalar(165,165,255));
		color_array.push_back(cv::Scalar(165,255,165));
		color_array.push_back(cv::Scalar(255,165,165));
		color_array.push_back(cv::Scalar(165,165,165));
		return;
	}
	
	
	void mode_switch(char input){
		mode = input;
		return;
	}
	double get_distance(std::pair<double,double> point1,std::pair<double,double> point2){
		return sqrt((point1.first - point2.first)*(point1.first - point2.first) + (point1.second - point2.second)*(point1.second - point2.second));
	}
	double get_distance(std::pair<int,int> point1,std::pair<int,int> point2){
		return sqrt((point1.first - point2.first)*(point1.first - point2.first) + (point1.second - point2.second)*(point1.second - point2.second));
	}
	void set_choose_id(int id){
		choose_id = id;
		return;
	}
	void insert_point(int x,int y){
		cout << choose_id << endl;
		if(point_map.find(choose_id) == point_map.end())
		point_map[choose_id] = empty;
		int dis_limit = 10;
		switch(mode){
			case 0:
			++total_point_num;
			point_set[total_point_num] = std::pair<int,int>(x,y);
			break;
			case 1:
			for(auto id_point:point_set){
				if(get_distance(id_point.second,std::pair<int,int>(x,y)) < dis_limit){
					if(std::find(point_map[choose_id].begin(),point_map[choose_id].end(),id_point.first) == point_map[choose_id].end())
					point_map[choose_id].push_back(id_point.first);
					break;
				}
			}
			break;
			default:break;
		}
		return;
	}
	void delete_point(int x,int y){
		if(point_map.find(choose_id) == point_map.end())
		point_map[choose_id] = empty;
		int dis_limit = 10;
		switch(mode){
			case 0:
			for(auto id_point:point_set){
				if(get_distance(id_point.second,std::pair<int,int>(x,y)) < dis_limit){
					cout << id_point.first << endl;
					point_set.erase(id_point.first);
					break;
				}
			}
			break;
			case 1:
			for(auto iter = point_map[choose_id].begin();iter != point_map[choose_id].end();){
				int id = *iter;
				if(get_distance(point_set[id],std::pair<int,int>(x,y)) < dis_limit){
					iter = point_map[choose_id].erase(iter);
					break;
				}
				else{
					++iter;
				}
			}
			
			for(auto id:point_map[choose_id]){
				cout << id << endl;
			}
			cout << endl;
			break;
			default:break;
		}
		return;
	}
	void shallow_init_map(cv::Mat my_map){
		map_img = my_map.clone();
		return;
	}
	void refresh_map(){
		for(auto it = point_map.begin();it != point_map.end(); ++it){
			for(auto it2 = it->second.begin(); it2 != it->second.end();){
				if(point_set.find(*it2) == point_set.end()){
					it2 = it->second.erase(it2);
				}
				else{
					++it2;
				}
			}
		}
	}
	void show_map(string window_name){
		cv::Mat show_img = map_img.clone();
		for(auto point:point_set){
			cv::circle(show_img,cv::Point(point.second.first,point.second.second),5,cv::Scalar(0,0,255),-1);
		}
		int CNT = 0;
		for(auto all_point:point_map){
			std::vector<int> point_vec = all_point.second;
			int size = point_vec.size();
			for(int i=0;i<size;++i){
				cv::line(show_img,
				cv::Point(point_set[point_vec[i]].first,point_set[point_vec[i]].second),
				cv::Point(point_set[point_vec[(i+1)%size]].first,point_set[point_vec[(i+1)%size]].second),
				color_array[CNT],2);
			}
			++CNT;
		}
		cv::imshow(window_name,show_img);
		return;
	}
	void save_map(std::string save_url_dir){
		{
			cv::Mat show_img = map_img.clone();
			for(auto point:point_set){
				cv::circle(show_img,cv::Point(point.second.first,point.second.second),5,cv::Scalar(0,0,255),-1);
			}
			int CNT = 0;
			for(auto all_point:point_map){
				std::vector<int> point_vec = all_point.second;
				int size = point_vec.size();
				for(int i=0;i<size;++i){
					cv::line(show_img,
					cv::Point(point_set[point_vec[i]].first,point_set[point_vec[i]].second),
					cv::Point(point_set[point_vec[(i+1)%size]].first,point_set[point_vec[(i+1)%size]].second),
					color_array[CNT],2);
				}
				++CNT;
			}

			std::string suffix = ".jpg";
			cv::imwrite((save_url_dir + suffix).c_str(),show_img);
		}
		std::string suffix = ".txt";
		FILE* file = fopen((save_url_dir + suffix).c_str(),"w");
		fprintf(file,"navigation_position = [");
		for(auto it = point_map.begin();it!=point_map.end();++it){
			auto point_vec = *it;
			int id = point_vec.first;
			std::vector<int> vec = point_vec.second;
			fprintf(file,"[");
			for(auto it = vec.begin();it != vec.end();){
				int point_id = *it;
				if(point_set.find(point_id) == point_set.end()){
					it = vec.erase(it);
				}
				else{
					auto point = point_set[point_id];
					double X,Y;
					X = 1.*point.first/save_rate_x;
					Y = expected_height - 1.*point.second/save_rate_y;
					++it;
					fprintf(file,"[%.2lf,%.2lf]",X,Y);
					if(it!=vec.end())
					fprintf(file,",");
				}
			}
			fprintf(file,"]");
			{
				auto it2 = it;
				++it2;
				if(it2 != point_map.end())
				fprintf(file,",");
			}
			fprintf(file,"\n");
		}
		fprintf(file,"]\n");
		return;
	}
	void set_save_rate(double rate_x,double rate_y){
		save_rate_x = rate_x;
		save_rate_y = rate_y;
		return;
	}
	void load_map_toml_file(std::string map_file){
		const auto toml_file = toml::parse(map_file);
		std::vector<std::vector<std::pair<double,double>>> navigation_position = toml::find<std::vector<std::vector<std::pair<double,double> > > >(toml_file,"navigation_position");
		
		const double eps = 0.01;

		int CNT = 0,now_id;

		point_set.clear();
		point_map.clear();

		for(auto position_vector:navigation_position){
			point_map[CNT] = empty;
			for(auto position:position_vector){
				std::pair<double,double> position_to,pdi_to;
				position_to = std::pair<double,double>(position.first * save_rate_x,(expected_height - position.second) * save_rate_y);
				for(auto pdi:point_set){
					pdi_to = std::pair<double,double>(pdi.second.first,pdi.second.second);
					if(get_distance(pdi_to,position_to) < eps){
						now_id = pdi.first;
						goto S;
					}
				}
				now_id = point_set.size();
				point_set[now_id] = position_to;
				++total_point_num;
				S:;
				if(std::find(point_map[CNT].begin(),point_map[CNT].end(),now_id) == point_map[CNT].end())
				point_map[CNT].push_back(now_id);
			}
			++CNT;
		}
		return;
	}
}point_chooser;

void mouse_callback(int event,int x,int y,int flags,void *userdata){
	if(event == cv::EVENT_LBUTTONDOWN){
		point_chooser.insert_point((double)x,(double)y);
	}
	else if(event == cv::EVENT_RBUTTONDOWN){
		point_chooser.delete_point((double)x,(double)y);
	}
	return;
}

int main(){

	const auto toml_file = toml::parse("../config/config.toml");
	
	std::string input_img_url = toml::find<std::string>(toml_file,"input_img_url"),
				output_toml_url = toml::find<std::string>(toml_file,"output_toml_url"),
				map_saver_url = toml::find<std::string>(toml_file,"map_saver_url");

	cv::Mat field_map = cv::imread(input_img_url);

	cv::namedWindow("DecisionArea",cv::WINDOW_AUTOSIZE);
	point_chooser = PointChooser();

	int expected_width = toml::find<int>(toml_file,"expected_width"),
		expected_height = toml::find<int>(toml_file,"expected_height");

	point_chooser.set_save_rate(1.*field_map.cols/expected_width,1.*field_map.rows/expected_height);
	point_chooser.set_hw(expected_height,expected_width);

	if(output_toml_url != "none_url")
	point_chooser.load_map_toml_file(output_toml_url);


	point_chooser.color_init();
	point_chooser.shallow_init_map(field_map);
	cv::setMouseCallback("DecisionArea",mouse_callback);

	int num = 0;


	while(1){
		point_chooser.show_map("DecisionArea");
		int type = cv::waitKey(10);
		point_chooser.refresh_map();
		if(type < 0) continue;
		if(type == 'q') break;
		if(type >= '0' && type <= '9'){
			num = num*10 + type - '0';
		}
		else{
			if(type == '\r'){
				point_chooser.set_choose_id(num);
				num = 0;
				continue;
			}
			num = 0;
			if(type == 'a')	type = 0;
			else			type = 1;
			point_chooser.mode_switch(type);
		}
	}
	point_chooser.save_map(map_saver_url);
	return 0;
}
