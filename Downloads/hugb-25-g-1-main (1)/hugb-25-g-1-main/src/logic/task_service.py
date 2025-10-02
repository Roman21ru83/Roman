from IO.taskIO import TaskIO
from models.task import Task


class TaskService:
    def __init__(self):
        self.io = TaskIO()
        self.tasks = self.io.load_tasks()

    def create_new_task(self, title: str, desc: str, due_date: str = None):
        new = Task(title, desc, due_date)
        self.tasks.append(new)
        self.io.write_task(new)
        return new

    def update_task_status(self, task: Task, status: int):
        task.updateStatus(status)
        return task

    def update_due_date(self, task: Task, due_date: str):
        task.update_due_date(due_date)
        return task
