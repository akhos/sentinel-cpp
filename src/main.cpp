#include <algorithm>
#include <cmath>
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
        long long m_char_counts[256] = {0};
        double m_char_probabilities[256] = {0.0};
        double m_entropy{0.0};

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

        int countChars()
        {
            std::ifstream file(m_filePath, std::ios::binary);
            if (!file)
            {
                std::cerr << "Could not open the file!" << std::endl;
                return 1; // Return an error code
            }
            std::vector<char> content(m_file_size);
            file.read(content.data(), m_file_size);

            for (char byte: content){
                m_char_counts[static_cast<unsigned char>(byte)]++;
            }
                return 0;
        }

        void calculateProbabilities() 
        {
            for (int i = 0 ; i < 256; i++) {
                if (m_char_counts[i] > 0) {
                    m_char_probabilities[i] = static_cast<double>(m_char_counts[i]) / m_file_size;
                }
                else {
                    m_char_probabilities[i] = 0.0;
                }
            }   
        }

        void calculateEntropy() {
            for (int i = 0 ; i < 256; i++) {
                if (m_char_probabilities[i] > 0) {
                    m_entropy -= m_char_probabilities[i] * std::log2(m_char_probabilities[i]);
                }

            }

        }

        double getEntropy() const {
            return m_entropy;
        }

        bool analyze()
        {
            extractFileInfo();
            if (countChars() != 0) {
                std::cerr << "Error counting characters." << std::endl;
                return false;
            }
            calculateProbabilities();
            calculateEntropy();
            return true;
        }
        
        void printReport() const 
        {
            std::cout << "\n--- Sentinel Analysis Report ---" << std::endl;
            std::cout << "File:    " << m_filePath.filename() << std::endl;
            std::cout << "Size:    " << m_file_size << " bytes" << std::endl;
            std::cout << "Entropy: " << std::fixed << std::setprecision(4) << m_entropy << std::endl;

            // The "Security Intelligence" Layer
            std::cout << "Status:  ";
            if (m_entropy > 7.5)      std::cout << "[CRITICAL] Encrypted/Highly Obfuscated" << std::endl;
            else if (m_entropy > 6.8) std::cout << "[WARNING] Compressed or Packed" << std::endl;
            else if (m_entropy > 4.5) std::cout << "[NORMAL] Standard Binary Data" << std::endl;
            else                      std::cout << "[SAFE] Low-entropy Text/Data" << std::endl;
            std::cout << "--------------------------------\n" << std::endl;
        }

 
};  


std::string getFileName() {
    std::string fName{};
    std::cout << "Enter the file name: ";
    std::cin>>fName;
    //return fName;
    return "/home/ali/CPP/wk1/src/temp.txt";
}   

int main() {
    Sentinel s{getFileName()};
    s.analyze();
    std::cout << "Entropy: " << s.getEntropy() << std::endl;

    return 0;
}   