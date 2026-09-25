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

class Program
{
    static void Main(string[] args)
    {
        Employee emp = new Employee("Вася", 1000, new DateTime(2022, 1, 1));
        Manager man = new Manager("Егор", 1000, new DateTime(2024, 1, 1));
        Developer dev = new Developer("Савелий", 1000, new DateTime(2024, 1, 1));
        Console.WriteLine(emp.CalculateSalary());
        Console.WriteLine(man.CalculateSalary());
        Console.WriteLine(dev.CalculateSalary());
    }
}