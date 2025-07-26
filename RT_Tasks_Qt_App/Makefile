CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -fPIC `pkg-config --cflags Qt5Widgets`
LDFLAGS = `pkg-config --libs Qt5Widgets`

SRCS = main.cpp mainwindow.cpp worker.cpp
MOC_SRCS = moc_mainwindow.cpp moc_worker.cpp
HEADERS = mainwindow.hpp worker.hpp

TARGET = rt_tasks_qt

all: $(TARGET)

# Генерация MOC-файлов (Qt метаобъект-компилятор)
moc_mainwindow.cpp: mainwindow.hpp
	moc $< -o $@

moc_worker.cpp: worker.hpp
	moc $< -o $@

# Линковка всей программы
$(TARGET): $(SRCS) $(MOC_SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(MOC_SRCS) $(LDFLAGS)

clean:
	rm -f *.o $(MOC_SRCS)