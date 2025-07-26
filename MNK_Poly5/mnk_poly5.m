% Запрос количества точек
n = input('Введите количество точек (не меньше 6): ');

if n < 6
    error('Для полинома 5-й степени нужно минимум 6 точек.');
end

x = zeros(n,1);
y = zeros(n,1);

% Ввод точек с клавиатуры
for i = 1:n
    x(i) = input(sprintf('Введите x(%d): ', i));
    y(i) = input(sprintf('Введите y(%d): ', i));
end

% Формируем матрицу для полинома степени 5
A = zeros(n, 6);
for i = 1:n
    for j = 0:5
        A(i,j+1) = x(i)^j;
    end
end

% Решаем систему МНК: находим коэффициенты полинома
% Решение методом нормальных уравнений: (A'A) * coeff = A'y
coeff = (A' * A) \ (A' * y);

% Формируем значения полинома для построения графика
x_plot = linspace(min(x), max(x), 200);
y_plot = zeros(size(x_plot));
for j = 0:5
    y_plot = y_plot + coeff(j+1)*x_plot.^j;
end

% Строим график
plot(x, y, 'ro', 'MarkerSize',8, 'DisplayName', 'Введённые точки');
hold on;
plot(x_plot, y_plot, 'b-', 'LineWidth', 2, 'DisplayName', 'Аппроксимирующий полином 5-й степени');
legend('Location', 'best');
grid on;
title('Аппроксимация методом МНК');
xlabel('x');
ylabel('y');
hold off;
