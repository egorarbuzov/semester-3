interface IShape
{
    double GetArea();
    double GetPerimeter();
}

abstract class Shape : IShape
{
    public string Color { get; set; }
    public abstract double GetArea();
    public abstract double GetPerimeter();
}

interface ISalesPerson : Shape
{
    string FirstName { get; set; }
    string LastName { get; set; }
}
