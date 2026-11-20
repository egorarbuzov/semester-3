class Employee
{
    public string Name { get; set; } = string.Empty;
    public string Department { get; set; } = string.Empty;
    public string Gender { get; set; } = string.Empty;
    public int Age { get; set; }
    public decimal Salary { get; set; }
}

class Program
{
    static void Main(string[] args)
    {
        List<Employee> employees = new List<Employee>
        {
            new Employee { Name = "Иван Петров", Department = "IT", Gender = "М", Age = 28, Salary = 65000m },
            new Employee { Name = "Анна Смирнова", Department = "IT", Gender = "Ж", Age = 32, Salary = 72000m },
            new Employee { Name = "Павел Козлов", Department = "IT", Gender = "М", Age = 41, Salary = 88000m },
            new Employee { Name = "Мария Иванова", Department = "IT", Gender = "Ж", Age = 26, Salary = 61000m },
            new Employee { Name = "Олег Орлов", Department = "IT", Gender = "М", Age = 36, Salary = 76000m },
            new Employee { Name = "Сергей Волков", Department = "Бухгалтерия", Gender = "М", Age = 45, Salary = 54000m },
            new Employee { Name = "Ольга Кузнецова", Department = "Бухгалтерия", Gender = "Ж", Age = 38, Salary = 58000m },
            new Employee { Name = "Елена Соколова", Department = "Бухгалтерия", Gender = "Ж", Age = 29, Salary = 51000m },
            new Employee { Name = "Дмитрий Морозов", Department = "Бухгалтерия", Gender = "М", Age = 51, Salary = 62000m },
            new Employee { Name = "Алексей Новиков", Department = "Отдел продаж", Gender = "М", Age = 34, Salary = 47000m },
            new Employee { Name = "Наталья Павлова", Department = "Отдел продаж", Gender = "Ж", Age = 27, Salary = 45000m },
            new Employee { Name = "Андрей Семенов", Department = "Отдел продаж", Gender = "М", Age = 39, Salary = 53000m }
        };

        var departmentGroups = employees
            .GroupBy(employee => employee.Department)
            .OrderBy(department => department.Key)
            .Select(department => new
            {
                Department = department.Key,
                GenderGroups = department
                    .GroupBy(employee => employee.Gender)
                    .OrderBy(gender => gender.Key)
                    .Select(gender => new
                    {
                        Gender = gender.Key,
                        Count = gender.Count(),
                        AverageAge = gender.Average(employee => employee.Age),
                        AverageSalary = gender.Average(employee => employee.Salary)
                    })
            });

        Console.WriteLine("{0,-15} {1,-6} {2,-14} {3,-18} {4,-18}",
            "Отдел", "Пол", "Сотрудников", "Средний возраст", "Средняя зарплата");
        Console.WriteLine(new string('-', 75));

        foreach (var department in departmentGroups)
        {
            foreach (var gender in department.GenderGroups)
            {
                Console.WriteLine("{0,-15} {1,-6} {2,-14} {3,-18:F1} {4,-18:F2}",
                    department.Department, gender.Gender, gender.Count,
                    gender.AverageAge, gender.AverageSalary);
            }
        }
    }
}
