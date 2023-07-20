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

    initscr();
    start_color();
    use_default_colors();

    int row, col;
    getmaxyx(stdscr, row, col); // get terminal size

    cv::resize(img, img, cv::Size(col, row)); // resize image to fit terminal size

    for(int i = 0; i < img.rows; i++) {
        for(int j = 0; j < img.cols; j++) {
            // map grayscale pixel value to ncurses color constant
            int color = COLOR_BLACK + (img.at<uchar>(i, j) / 32); 
            init_pair(i * img.cols + j + 1, color, COLOR_BLACK);
            attron(COLOR_PAIR(i * img.cols + j + 1));
            printw("%s", pixelToAscii(img, i, j).c_str());
            attroff(COLOR_PAIR(i * img.cols + j + 1));
        }
        printw("\n");
    }

    refresh();
    getch();
    endwin();

    return 0;
}
