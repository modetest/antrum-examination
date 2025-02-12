#pragma once

#include <vector>  
#include <iostream>  
#include <limits>  
#include <map>  
#include <filesystem>

#include <qobject.h>

#include <opencv2/opencv.hpp>


class matchRuler : public QObject
{
	Q_OBJECT
public:
	matchRuler(const std::string path,QObject* parent = nullptr);
	~matchRuler();
public:

	bool AutoMatchRuler(cv::Mat& image_file, float& unit);

private:
	bool getTemplateFilse(const std::string& template_path);
	std::pair<double, float> one_matches(const std::string& template_file,  cv::Mat& image_file);

private:
	QVector<std::string> template_files;

};

