from models.task import Task
from IO import TaskIO


def main():
    task1 = Task("test task", "This task is to do nothing")
    IO = TaskIO()
    IO.write_task_to_file(task1)
    print("this ran")


if __name__ == '__main__':
    main()
