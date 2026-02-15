#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>

struct FileInfo {
    std::string name;
    std::string extension;
    std::filesystem::path path{};
    long long size;
};

class Sentinel{
    private:
        FileInfo m_fileInfo{};
        long long m_char_counts[256] = {0};
        double m_char_probabilities[256] = {0.0};
        double m_entropy{0.0};

    public:
        explicit Sentinel(std::string_view fileName):
        m_fileInfo{.path = fileName}
        {
        }

        void extractFileInfo() // Extracts file information such as name, extension, and size
        {
            if (std::filesystem::exists(m_fileInfo.path)) {
                m_fileInfo.name = m_fileInfo.path.filename().string();
                m_fileInfo.extension = m_fileInfo.path.extension();
                m_fileInfo.size = std::filesystem::file_size(m_fileInfo.path);
                std::cout << m_fileInfo.size << " bytes" << std::endl;
            } else {
                std::cout << "No file name found." << std::endl;
            }
        }

        std::vector<char> readFileContent() // Returns the file content as a vector of bytes (chars)
        {
            std::ifstream file(m_fileInfo.path, std::ios::binary);
            if (!file)
            {
                std::cerr << "Could not open the file!" << std::endl;
                return {}; // Return empty vector on error
            }
            std::vector<char> content(m_fileInfo.size);
            file.read(content.data(), m_fileInfo.size);
            return content;
        }

        std::vector<double> calculateSlidingWindowEntropy(const std::vector<char>& content) // calculates and returns the entropy of each window in a file
        {
            const size_t window_size = 1024;
            std::vector<double> window_entropies;
            for (size_t i = 0; i < content.size(); i += window_size) {
                size_t current_window_size = std::min(window_size, content.size() - i);
                long long window_char_counts[256] = {0};

                for (size_t j = 0; j < current_window_size; ++j) {
                    window_char_counts[static_cast<unsigned char>(content[i + j])]++;
                }

                double window_entropy = 0.0;
                window_entropy = calculateEntropy(window_char_counts, current_window_size); 
                window_entropies.push_back(window_entropy);
            }   
            return window_entropies;
        }

        double calculateFileEntropy(const std::vector<char>& content)
        {
            const size_t total_chars = content.size();
            long long char_counts[256] = {0};
            double m_entropy = 0.0;
                
            for (size_t j = 0; j < total_chars; ++j) {
                char_counts[static_cast<unsigned char>(content[j])]++;
            }

            m_entropy = calculateEntropy(char_counts, total_chars);
            return m_entropy;
        }

        double calculateEntropy(const long long char_counts[256], size_t total_chars) {
            double entropy = 0.0;
            for (int k = 0; k < 256; ++k) {
                if (char_counts[k] > 0) {
                    double probability = static_cast<double>(char_counts[k]) / total_chars;
                    entropy -= probability * std::log2(probability);
                }
            }
            return entropy;
        }

        FileInfo getFileInfo() const {
            return m_fileInfo;
        }

        void printReport(double totalEntropy, const std::vector<double>& windowEntropies, const FileInfo& fileInfo) {
            std::cout << "\n==========================================" << std::endl;
            std::cout << "       SENTINEL ANALYSIS REPORT           " << std::endl;
            std::cout << "==========================================" << std::endl;
            std::cout << "File Name: " << fileInfo.name << std::endl;
            std::cout << "Extension: " << fileInfo.extension << std::endl;
            std::cout << "File Size: " << fileInfo.size << " bytes" << std::endl;
            std::cout << "Total File Entropy: " << std::fixed << std::setprecision(4) << totalEntropy << std::endl;
            std::cout << "------------------------------------------" << std::endl;

            bool suspiciousFound = false;
            const size_t window_size = 1024; // Matches your calculation window

            std::cout << "Scanning for suspicious segments..." << std::endl;
            for (size_t i = 0; i < windowEntropies.size(); ++i) {
                double entropy = windowEntropies[i];
                
                // Threshold: 7.5 is usually the "smoking gun" for encryption
                if (entropy > 7.5) {
                    std::cout << "[!] ALERT: High Entropy (" << entropy << ") at Offset: 0x" 
                            << std::hex << (i * window_size) << std::dec << std::endl;
                    suspiciousFound = true;
                }
            }

            if (!suspiciousFound) {
                std::cout << "[+] No suspicious spikes detected." << std::endl;
            }

            std::cout << "==========================================\n" << std::endl;
        }  
};  


std::string getFileName() {
    std::string fName{};
    std::cout << "Enter the file name: ";
    std::cin>>fName;
    //return fName;
    return "/home/ali/CPP/wk1/src/temp.txt";
}   

void exportToJson(double totalEntropy, const std::vector<double>& windowEntropies, const FileInfo& fileInfo) {
    std::ofstream jsonFile("scan_results.json");
    if (!jsonFile) {
        std::cerr << "Could not create JSON file!" << std::endl;
        return;
    }

    jsonFile << "{\n";
    jsonFile << "  \"filename\": \"" << fileInfo.name << "\",\n";
    jsonFile << "  \"extension\": \"" << fileInfo.extension << "\",\n";
    jsonFile << "  \"file_size\": " << fileInfo.size << ",\n";
    jsonFile << "  \"total_entropy\": " << totalEntropy << ",\n";
    jsonFile << "  \"window_entropies\": [";
    
    for (size_t i = 0; i < windowEntropies.size(); ++i) {
        jsonFile << windowEntropies[i] << (i == windowEntropies.size() - 1 ? "" : ", ");
    }
    
    jsonFile << "]\n";
    jsonFile << "}";
    
    jsonFile.close();
    std::cout << "[+] Data exported to scan_results.json" << std::endl;
}

int main() {
    Sentinel s{getFileName()};
    
    s.extractFileInfo();
    std::vector<char> content = s.readFileContent();
    
    if (!content.empty()) {
        double total = s.calculateFileEntropy(content);
        std::vector<double> windows = s.calculateSlidingWindowEntropy(content);
        FileInfo fileInfo = s.getFileInfo();
        exportToJson(total, windows, fileInfo);
        s.printReport(total, windows, fileInfo);
    }



    return 0;
}   