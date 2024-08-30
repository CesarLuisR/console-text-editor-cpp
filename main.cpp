#include <iostream>
#include <string>
#include <limits>
#include <filesystem>
#include <fstream>

struct file_response {
  std::string content;
  std::string state;
};

void render_window(std::string header) {
  system("cls");
  std::cout << header << "\n\n";
}

int get_option(std::string prompt = "Choose an option: ") {
  int option = 0;
  std::cout << prompt;
  std::cin >> option;
  std::cin.ignore(INT_MAX, '\n');

  if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(INT_MAX, '\n');
    return get_option("Invalid input, please enter a number: ");
  }

  if (option == 1 || option == 2 || option == 3) 
    return option;

  return get_option("Incorrect option, try again: ");
}

std::string get_file_name() {
  std::string file_name;
  std::cout << "Enter file name: ";
  std::getline(std::cin, file_name);
  return file_name;
}

std::string get_file_content(std::string file) {
  std::ifstream in_file;
  std::string line, content;

  in_file.open(file);
  while(!in_file.eof()) {
    std::getline(in_file, line);
    content += line + '\n';
  }
  
  in_file.close();
  return content;
}

file_response write_in_file() {
  std::string line, text;
  while (true) {
    std::getline(std::cin, line);
    if (line == ":q" || line == ":s") break;
    text += line + '\n';
  }
  return {text, line};
}

bool does_file_exits(std::string file) {
  const std::string dir = "./files/" + file;
  std::ifstream in_file;

  in_file.open(dir);
  if (!in_file.fail()) {
    in_file.close();
    return true;
  }
  in_file.close();

  return false;
};

void open_file(std::string file = "") {
  std::ifstream in_file;
  std::string dir;

  while (true) {
    if (file == "") file = get_file_name();
    dir = "./files/" + file;

    if (!does_file_exits(file)) {
      std::cout << "This file doesn't exits, try again \n";
      file = "";
      continue;
    }

    break;
  }

  render_window("You at " + file);
  std::ofstream out_file(dir, std::ofstream::app);

  std::cout << get_file_content(dir);
  file_response res = write_in_file();

  if (res.state == ":s") {
    out_file << res.content;
    out_file.close();
    return;
  }

  out_file.close();
}

void create_file() {
  if (!std::filesystem::exists("./files"))
    std::filesystem::create_directory("./files");

  std::ifstream in_file;
  std::ofstream out_file;
  std::string file, dir;

  while (true) {
    file = get_file_name();
    dir = "./files/" + file;

    if(does_file_exits(file)) {
      std::cout << "This file alredy exits, try again \n";
      continue;
    }

    out_file.open(dir);
    out_file.close();

    break;
  }

  render_window("You at " + file);
  open_file(file);
}

void logic() {
  render_window("Text editor \n Create a file: 1 \n Open a file: 2 \n Exit: 3");
  int option = get_option();

  switch (option) {
  case 1:
    render_window("Creating a file... ");
    create_file();
    break;

  case 2:
    render_window("Opening a file...");
    open_file();

  case 3:
    std::exit(0);
  }
}

int main() {
  while (true) 
    logic();
};