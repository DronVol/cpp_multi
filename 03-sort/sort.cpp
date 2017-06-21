#include <iostream>
#include <queue>        
#include <string> 
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <sstream> 
#include <stdio.h>
#include <cstdlib>
 
using namespace std;

class Sort{
	public:
		Sort(int buf_size, int sort_num){
			buf_len = buf_size;
			sort_number = sort_num;		
			batch_size = buf_len/(sort_number + 1);
		}
		
		~Sort(){
			del();
		};
		
		void del(){
			// по окончании работы почистим мусор
			system("rm -rf tmp");
			cout << "tmp folder removed.\n";
		}

		void init(const char * INPUT_FILE){
			// Разбиваем наш большой файл на множество отсортированных размером >= buf_len
			system("mkdir tmp");

			ifstream FileIn(INPUT_FILE, ios:: in | ios::binary);				
			FileIn.seekg(0, FileIn.end);
			size_t len = FileIn.tellg(); // длина файла 
			size_t num = 0; // номер текущего файла вывода
			size_t position = 0; // смещение в исходном файле

			while(len > position){
				FileIn.seekg(position);

				// количество байт, которые мы запишем в новый файл
				size_t l = buf_len;
				if(buf_len > len - position)
					l = len - position;

				// считываем батч
				vector<int> buf(l / sizeof(int));
				FileIn.read(reinterpret_cast<char*>(buf.data()), buf.size()*sizeof(int)); 

				//сортируем
				sort(buf.begin(), buf.end());

				//выводим в новый файл				
				stringstream  adr;
				adr <<  "tmp/init" << num;
				ofstream FileOut;
				FileOut.open(adr.str(), ios::out | ofstream::binary);
				FileOut.write(reinterpret_cast<const char*>(&buf[0]), buf.size() * sizeof(int));
				FileOut.close();
				position += buf_len;
				num++;

				//добавляем новый файл в очередь
				FILES.push(adr.str());
			}
			FileIn.close();
		}

		void file_sort(){
			// сортировка множества файлов
			int num = 0; // номер нового файла после merge
			while (FILES.size() > 1){

				cout << "FILES LEFT: "<< FILES.size() << "\n";
				vector<string> files; // батч файлов для merge

				for (int i=0; (i<sort_number) && (i <= FILES.size()) ; ++i){	
					files.push_back(FILES.front());
					FILES.pop();
				}

				merge(files, num);
				files.clear();
				
				stringstream  adr;
				adr <<  "tmp/new" << num;
				FILES.push(adr.str());
				num++;
			}

			// по окончании скопируем оставшийся "собранный" файл в качестве result
			stringstream  cmd;
			cmd <<  "cp " << FILES.front() << " result";
			const string tmp = cmd.str();
			const char* cstr = tmp.c_str();
			cout << cstr << "\n";
			system(cstr);
			
		}

		void merge(vector<string> files, int k){

			vector<vector<int> > buf(files.size()); // буфер
			vector<int> len(files.size()); // оставшиеся длины файлов для чтения
			vector<int> position(files.size()); // позиция с которой читать
			vector<int> sorted(0); // отсортированный вектор вывода
			vector<bool> check(files.size()); // проверка на то, что  считываемый файл закончился
			vector<size_t> place(files.size()); // текущее место в буфере для каждого файла


			// считываем batсh из каждого файла и определяем начальные значения
			for (int i=0; i<buf.size(); ++i){
				ifstream FileIn(files[i], ios::in | ios::binary);
				FileIn.seekg(0, FileIn.end);
				len[i] = FileIn.tellg(); 
				FileIn.seekg(0);

				position[i] = min(batch_size, int(len[i]/sizeof(int)));
				cout << files[i] << " " << len[i] << "\n";
				len[i] -= position[i] * sizeof(int);
 				buf[i].resize(position[i]);			

				FileIn.read(reinterpret_cast<char*>(buf[i].data()), buf[i].size() * sizeof(int)); 
				FileIn.close();

 				place[i] = 0;
				check[i] = true;
			}

			bool full_check = true;	// проверка на то, что все файлы закончились			

			
			while (full_check){
				int minimum = INT_MAX;
				size_t min_pos = -1;

				// определяем минимальный элемент в текущей "строке"
				for (int i=0; i<buf.size(); i++){
					if ((check[i] == true) && (minimum >= buf[i][place[i]])){
						minimum = buf[i][place[i]];
						min_pos = i;
					}
				}
				
				// добавили значение в отсортированнын
				sorted.push_back(buf[min_pos][place[min_pos]]);
				place[min_pos] += 1;

				// если файл считанный буфер закончился
				if (place[min_pos] >= buf[min_pos].size()){
					
					// если файл считали полностью
					if (len[min_pos] < 1) {						
						check[min_pos] = false;
					}
					else{

					place[min_pos] = 0;

					ifstream FileIn(files[min_pos], ios::in | ios::binary);
					FileIn.seekg(position[min_pos] * sizeof(int));

					size_t pos = min(batch_size, int(len[min_pos]/sizeof(int)));
					len[min_pos] -= pos * sizeof(int);
					position[min_pos] += pos;

					buf[min_pos].clear();
					buf[min_pos].resize(pos);
					
					FileIn.read(reinterpret_cast<char*>(buf[min_pos].data()), buf[min_pos].size() * sizeof(int));

					}
				}

				// если буфер вывода заполнился
				if (sorted.size() >= batch_size / sizeof(int)){
					stringstream  adr;
					adr <<  "tmp/new" << k;
					ofstream FileOut(adr.str(), ios::out | ios::app | ios::binary);
					FileOut.write(reinterpret_cast<const char*>(&sorted[0]), sorted.size() * sizeof(int));

					sorted.clear();
					sorted.resize(0);

				}

				// проверяем, что есть ещё, что обрабатывать
				full_check = false;
				for (int i=0;i<check.size();i++)
					full_check += check[i];
			}

			
			// выведем, если ещё что-то осталось в отсортированном буфере
			stringstream  adr;
			adr <<  "tmp/new" << k;

			ofstream FileOut(adr.str(), ios::out | ios::app | ios::binary);
			FileOut.write(reinterpret_cast<const char*>(&sorted[0]), sorted.size() * sizeof(int));

			sorted.clear();

		}
	private:
		int buf_len; // размер буфера
		int sort_number; // количество файлов при сортировке
		int batch_size; // размер сортировочного батча
		queue<string> FILES; // очередь файлов
};



int main(int argc, char** argv)
{
	size_t KB = 1024 ;
	size_t buf_size = (size_t) atoll(argv[3]) * KB; 
	size_t sort_files = (size_t) atoll(argv[2]); 

	Sort s(buf_size, sort_files);
	s.init(argv[1]);
	s.file_sort();

    return 0;
}