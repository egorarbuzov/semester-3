using System;

class UnknownOperationException : Exception
{
    public string Operation { get; set; }

    public UnknownOperationException() : base() { }

    public UnknownOperationException(string message) : base(message) { }

    public UnknownOperationException(string message, Exception inner) : base(message, inner) { }
}

class Calculator
{
    public double Add(double a, double b)
    {
        return a + b;
    }

    public double Subtract(double a, double b)
    {
        return a - b;
    }

    public double Multiply(double a, double b)
    {
        return a * b;
    }

    public double Divide(double a, double b)
    {
        if (b == 0)
        {
            throw new DivideByZeroException("Деление на ноль невозможно");
        }
        else
        {
            return a / b;
        }
    }
}

class Program
{
    static void Main(string[] args)
    {
        Console.WriteLine("--- Калькулятор ---");
        Calculator calc = new Calculator();

        bool exit = false;

        while (!exit)
        {
            try
            {
                Console.Write("Введите операцию (+, -, *, /) или 'exit': ");
                string op = Console.ReadLine();

                if (op == "exit")
                {
                    exit = true;
                    continue;
                }

                Console.Write("Введите первое число: ");
                double a = double.Parse(Console.ReadLine());

                Console.Write("Введите второе число: ");
                double b = double.Parse(Console.ReadLine());

                double result = 0;

                if (op == "+")
                {
                    result = calc.Add(a, b);
                }
                else if (op == "-")
                {
                    result = calc.Subtract(a, b);
                }
                else if (op == "*")
                {
                    result = calc.Multiply(a, b);
                }
                else if (op == "/")
                {
                    result = calc.Divide(a, b);
                }
                else
                {
                    UnknownOperationException ex = new UnknownOperationException(
                        $"Неизвестная операция: {op}");
                    ex.Operation = op;
                    throw ex;
                }

                Console.WriteLine($"Результат: {a} {op} {b} = {result}");
            }
            catch (DivideByZeroException ex)
            {
                Console.WriteLine($"Ошибка: {ex.Message}");
            }
            catch (FormatException ex)
            {
                Console.WriteLine($"Ошибка: неверный формат числа! {ex.Message}");
            }
            catch (OverflowException ex)
            {
                Console.WriteLine($"Ошибка: переполнение! {ex.Message}");
            }
            catch (UnknownOperationException ex)
            {
                Console.WriteLine($"Ошибка: {ex.Message}");
                Console.WriteLine($"Некорректная операция: {ex.Operation}");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Неизвестная ошибка: {ex.Message}");
            }
            finally
            {
                Console.WriteLine("Блок finally выполнен (очистка ресурсов)");
            }

            Console.WriteLine();
        }

        Console.WriteLine("Программа завершена.");
    }
}