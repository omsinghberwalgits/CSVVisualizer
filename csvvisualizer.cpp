#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <string>

using namespace std;

// Structure to hold the data from the CSV file
struct Data {
    string category;
    float value;
};

// Function to read data from a CSV file
vector<Data> readCSV(const string& filename) {
    vector<Data> data;
    ifstream file(filename);
    
    string line;
    
    // Skip the header line
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string category;
        float value;

        getline(ss, category, ',');
        ss >> value;
        
        data.push_back({category, value});
    }

    return data;
}

// Function to draw a pie chart
void drawPieChart(sf::RenderWindow& window, const vector<Data>& data) {
    // Calculate the total sum of values
    float totalValue = 0;
    for (const auto& d : data) {
        totalValue += d.value;
    }

    // Create a pie chart
    float startAngle = 0.0f;
    for (const auto& d : data) {
        float percentage = (d.value / totalValue) * 360.0f;
        
        // Create a wedge (sector of the pie)
        sf::CircleShape pieSlice(200, 50);  // radius = 200, number of points = 50 for smoothness
        pieSlice.setPointCount(50);
        pieSlice.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));  // Random colors
        pieSlice.setRotation(startAngle);  // Rotate to the starting angle of the slice
        pieSlice.setPosition(300, 300);  // Position the pie chart at the center
        
        // Rotate the wedge to the correct angle
        float angle = startAngle + percentage;
        pieSlice.setPointCount(50);  // Ensure a smooth shape
        
        window.draw(pieSlice);
        startAngle = angle;
    }
}

// Function to draw a bar chart
void drawBarChart(sf::RenderWindow& window, const vector<Data>& data) {
    float barWidth = 50.0f;
    float spaceBetweenBars = 20.0f;
    
    // Find max value to normalize the bars
    float maxValue = 0;
    for (const auto& d : data) {
        if (d.value > maxValue) {
            maxValue = d.value;
        }
    }

    // Draw the bars
    float xPos = 100.0f;
    for (const auto& d : data) {
        float barHeight = (d.value / maxValue) * 400.0f;  // Normalize height to fit the window
        
        sf::RectangleShape bar(sf::Vector2f(barWidth, barHeight));
        bar.setPosition(xPos, 500 - barHeight);  // Align bars at the bottom of the window
        bar.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));  // Random colors
        
        window.draw(bar);
        
        // Add text label
        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {  // Make sure to have arial.ttf in the same folder
            cout << "Error loading font!" << endl;
            return;
        }
        
        sf::Text label(d.category, font, 20);
        label.setPosition(xPos, 510);
        label.setFillColor(sf::Color::Black);
        window.draw(label);

        xPos += barWidth + spaceBetweenBars;
    }
}

int main() {
    // Read the CSV file
    string filename = "data.csv";
    vector<Data> data = readCSV(filename);
    
    // Create a window to display the charts
    sf::RenderWindow window(sf::VideoMode(800, 600), "CSV Data Visualization");
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        window.clear(sf::Color::White);  // Clear the window with white color
        
        // Draw the pie chart and bar chart
        drawPieChart(window, data);
        drawBarChart(window, data);
        
        window.display();  // Display the updated window
    }
    
    return 0;
}
