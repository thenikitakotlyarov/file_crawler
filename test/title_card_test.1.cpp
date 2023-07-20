#include <opencv2/opencv.hpp>
#include <ncurses.h>
#include <string>
#include <iostream>

std::string pixelToAscii(cv::Mat &img, int row, int col) {
    int pixel = img.at<uchar>(row, col);
    std::string asciiChars = "@%#*+=-:. ";

    return std::string(1, asciiChars[pixel / (256 / asciiChars.size())]);
}

int main() {
    cv::Mat img = cv::imread("titlecard.jpg", cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    cv::resize(img, img, cv::Size(), 0.1, 0.1);

    initscr();
    start_color();
    use_default_colors();

    for(int i = 0; i < img.rows; i++) {
        for(int j = 0; j < img.cols; j++) {
            printw("%s", pixelToAscii(img, i, j).c_str());
        }
        printw("\n");
    }

    refresh();
    getch();
    endwin();

    return 0;
}
