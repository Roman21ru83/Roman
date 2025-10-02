from dataclasses import dataclass
from typing import Optional
from datetime import date


@dataclass
class Task:
    # Unique identifier for the task (e.g., "t1", "t2")
    id: str
    # Short descriptive title of the task
    title: str
    # Current status of the task (default is "todo")
    status: str = "todo"
    # Optional due date for the task (None if not set)
    due_date: Optional[date] = None
