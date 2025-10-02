from .task_model import Task


class TaskRepo:
    def __init__(self):
        # Store tasks in memory as a dictionary {id: Task}
        self._data = {}
        self._seq = 0  # simple numeric ID generator

    def next_id(self) -> str:
        """Return the next ID in the format t1, t2, ..."""
        self._seq += 1
        return f"t{self._seq}"

    def add(self, task: Task) -> Task:
        """Add a task to the repository"""
        self._data[task.id] = task
        return task

    def get(self, task_id: str) -> Task:
        """Find a task by ID, raise KeyError if not found"""
        if task_id not in self._data:
            raise KeyError("Task not found")
        return self._data[task_id]

    def all(self):
        """Return a list of all tasks"""
        return list(self._data.values())
