#include <algorithm>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>

class Sentinel{
    private:
        std::string m_filename{};
        std::filesystem::path m_filePath{};
        std::string m_file_extention{};
        std::string m_file_name{};
        long long m_file_size{0};

    public:
        explicit Sentinel(std::string_view fileName):
        m_filePath(fileName)
        {
        }

        void extractFileInfo() {
            if (std::filesystem::exists(m_filePath)) {
                m_file_name = m_filePath.filename().string();
                m_file_extention = m_filePath.extension();
                m_file_size = std::filesystem::file_size(m_filePath);
                std::cout << m_file_size << " bytes" << std::endl;
            } else {
                std::cout << "No file name found." << std::endl;
            }
        }

        int findPattern() {
            // Implement pattern finding logic here
            std::ifstream file(m_filePath, std::ios::binary);
            std::cout << "Finding pattern in the file..." << m_filePath << std::endl;
            if (!file)
            {
                std::cerr << "Could not open the file!" << std::endl;
                return 1; // Return an error code
            }

            std::vector<char> content(m_file_size);
            file.read(content.data(), m_file_size);
            // Process the content to find the pattern
            long long count {processFileContent(content)};

            std::cout << "Pattern found " << count << " times." << std::endl;

            return 0; // Placeholder return value
        };

        long long processFileContent(const std::vector<char>& content)
        {
            long long count  = std::count(content.begin(), content.end(), 0x00);
            return count;
        }
 
};  


std::string getFileName() {
    std::string fName{};
    std::cout << "Enter the file name: ";
    std::cin>>fName;
    //return fName;
    return "/home/ali/CPP/wk1/sentinel/temp.txt";
}   

int main() {
    Sentinel s{getFileName()};
    s.extractFileInfo();
    s.findPattern();
    return 0;
}   