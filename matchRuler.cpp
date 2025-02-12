
#include "matchRuler.h"

matchRuler::matchRuler(const std::string path, QObject* parent)
{
    if (!getTemplateFilse(path))
        std::cout << "read template image error!" << std::endl;
}

matchRuler::~matchRuler()
{

}

bool matchRuler::AutoMatchRuler(cv::Mat& image_file,float& unit)
{
    double max_threshold = 0.89;
    for (const auto& template_file : template_files) {
        auto result = one_matches(template_file, image_file);
        // ���������ֵ�͵�λ  
        if (result.first > max_threshold) {
            //std::cout << result.first << std::endl;
            unit = result.second;
        }
    }

    if (unit > 0) {
        std::cout << "The most similar template unit=" << unit << " ����ÿ����" << std::endl;
        return true;
    }
    else {
        std::cout << "No template found above the threshold." << std::endl;
        return false;
    }
}

std::pair<double, float> matchRuler::one_matches( const std::string& template_file, cv::Mat& image)
{
    cv::Mat template_mat = cv::imread(template_file, cv::IMREAD_ANYDEPTH | cv::IMREAD_COLOR);
    if (template_mat.empty()) {
        std::cerr << "Error loading template: " << template_file << std::endl;
        return  { 0.0, 0.0f };
    }
    // ���ߴ��Ƿ���ͬ  
    if (image.size() != template_mat.size()) {
        // �����ͬ�����ص�λ0 �� ���Ŷ� 0  
        return { 0.0, 0.0f };
    }
    // Convert to BGR (OpenCV default color format)  
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
    cv::cvtColor(template_mat, template_mat, cv::COLOR_RGB2BGR);

    // ��ֵ��ģ��  
    cv::Mat binary_template;
    cv::cvtColor(template_mat, binary_template, cv::COLOR_BGR2GRAY);
    cv::threshold(binary_template, binary_template, 2, 255, cv::THRESH_BINARY);

    // Ӧ������ 
    cv::Mat mask = cv::Mat::zeros(binary_template.size(), CV_8UC1);
    binary_template.copyTo(mask, binary_template == 255);

    // Apply mask to the image  
    cv::Mat masked_image;
    image.copyTo(masked_image, mask);

    // Match template  
    cv::Mat result;
    cv::matchTemplate(masked_image, template_mat, result, cv::TM_CCOEFF_NORMED);

    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

    // ��ȡ��λ����  
    // �ҵ����һ��б�ܵ�λ��  
    size_t found = template_file.find_last_of("/\\");
    // ����ҵ���б�ܣ���ȡб��֮��Ĳ�����Ϊ�ļ���  
    if (found != std::string::npos) {
        std::string filename = template_file.substr(found + 1);
    }
    else {
        std::cout << "·����δ�ҵ�б�ܡ�" << std::endl;
    }

    size_t dot_pos = template_file.rfind('.');
    float unit = std::stof(template_file.substr(found + 1, dot_pos));

    //std::cout << unit << std::endl;
    return  { maxVal, unit };
}

bool matchRuler::getTemplateFilse(const std::string& template_path)
{
	if (!std::filesystem::exists(template_path))
		return false;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(template_path)) {
        auto filePath = entry.path().string();
        auto filenameStr = entry.path().filename().string();
        auto extension = entry.path().extension();
        if (entry.is_regular_file()) {

            if (extension == ".png")
            {
                template_files.append(filePath);
            }

        }
    }

    if (template_files.size() == 0)
        return false;

	return true;
}
