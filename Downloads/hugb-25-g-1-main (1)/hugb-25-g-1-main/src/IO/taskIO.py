# src/IO/taskIO.py
import csv
import os
from models.task import Task

TASK_CSV_PATH = "src/data/tasks.csv"


class TaskIO:
    """Handles simple Task I/O with a CSV file."""

    def _ensure_parent_dir(self) -> None:
        """Make sure the parent folder exists."""
        os.makedirs(os.path.dirname(TASK_CSV_PATH), exist_ok=True)

    def write_task(self, task: Task) -> None:
        """Append (save) a single task to the CSV file."""
        self._ensure_parent_dir()
        with open(TASK_CSV_PATH, mode="a", newline="", encoding="utf-8") as file:
            fieldnames = ["id", "title", "desc", "status", "due_date"]
            writer = csv.DictWriter(file, fieldnames=fieldnames)
            if file.tell() == 0:  # add header if file is empty
                writer.writeheader()
            writer.writerow({
                "id": task.id,
                "title": task.title,
                "desc": task.desc,
                "status": task.status,
                "due_date": task.due_date or ""
            })

    def load_tasks(self) -> list[Task]:
        """Load all tasks from the CSV file. If file doesn't exist, return empty list."""
        tasks: list[Task] = []
        try:
            with open(TASK_CSV_PATH, mode="r", newline="", encoding="utf-8") as file:
                reader = csv.DictReader(file)
                for row in reader:
                    tasks.append(Task(
                        id=row["id"],
                        title=row["title"],
                        desc=row["desc"],
                        status=row["status"],
                        due_date=row["due_date"] or None
                    ))
        except FileNotFoundError:
            return []
        return tasks

    def write_all(self, tasks: list[Task]) -> None:
        """Overwrite the CSV file with the full list of tasks."""
        self._ensure_parent_dir()
        fieldnames = ["id", "title", "desc", "status", "due_date"]
        with open(TASK_CSV_PATH, mode="w", newline="", encoding="utf-8") as file:
            writer = csv.DictWriter(file, fieldnames=fieldnames)
            writer.writeheader()
            for t in tasks:
                writer.writerow({
                    "id": t.id,
                    "title": t.title,
                    "desc": t.desc,
                    "status": t.status,
                    "due_date": t.due_date or ""
                })

    def delete_task_by_id(self, task_id: str) -> bool:
        """
        Delete a task with the given ID from the CSV.
        Returns True if a task was deleted, False otherwise.
        """
        tasks = self.load_tasks()
        new_tasks = [t for t in tasks if t.id != task_id]
        deleted = len(new_tasks) != len(tasks)
        if deleted:
            self.write_all(new_tasks)
        return deleted
