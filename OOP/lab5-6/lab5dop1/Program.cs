class Employee
{
    public string Name { get; set; }
    public double BaseSalary { get; set; }
    public DateTime HireDate { get; set; }

    public Employee(string Name, double BaseSalary, DateTime HireDate)
    {
        this.Name = Name;
        this.BaseSalary = BaseSalary;
        this.HireDate = HireDate;
    }

    public virtual double CalculateSalary()
    {
        return BaseSalary;
    }
    
    public static double GetTotalSalary(Employee[] employees)
    {
        double total = 0;
        foreach (Employee e in employees)
        {
            total += e.CalculateSalary();
        }
        return total;
    }
    
    public static Employee FindHighestPaid(Employee[] employees)
    {
        Employee best = employees[0];
        foreach (Employee e in employees)
        {
            if (e.CalculateSalary() > best.CalculateSalary())
            {
                best = e;
            }
        }
        return best;
    }
}

class Manager : Employee
{
    public Manager(string Name, double BaseSalary, DateTime HireDate) : base(Name, BaseSalary, HireDate) {}

    public override double CalculateSalary()
    {
        return BaseSalary * 1.2;
    }
}

class Developer : Employee
{
    public Developer(string Name, double BaseSalary, DateTime HireDate) : base(Name, BaseSalary, HireDate) {}

    public override double CalculateSalary()
    {
        return BaseSalary * 1.1 + (500 * (DateTime.Today.Year - HireDate.Year));
    }
}

class SalesPerson : Employee
{
    public double SalesVolume { get; set; }

    public SalesPerson(string Name, double BaseSalary, DateTime HireDate, double SalesVolume)
        : base(Name, BaseSalary, HireDate)
    {
        this.SalesVolume = SalesVolume;
    }

    public override double CalculateSalary()
    {
        return BaseSalary + SalesVolume * 0.05;
    }
}

class Program
{
    static void Main(string[] args)
    {
        Employee[] employees =
        {
            new Employee("Вася", 1000, new DateTime(2022, 1, 1)),
            new Manager("Егор", 1000, new DateTime(2024, 1, 1)),
            new Developer("Савелий", 1000, new DateTime(2024, 1, 1)),
            new SalesPerson("Оля", 1000, new DateTime(2023, 1, 1), 20000)
        };
        
        foreach (Employee e in employees)
        {
            Console.WriteLine($"{e.Name}: {e.CalculateSalary()}");
        }

        Console.WriteLine($"\nОбщая сумма: {Employee.GetTotalSalary(employees)}");

        Employee best = Employee.FindHighestPaid(employees);
        Console.WriteLine($"Максимальная зарплата у {best.Name}: {best.CalculateSalary()}");
    }
}