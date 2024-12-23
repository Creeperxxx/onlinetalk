DELIMITER $$

CREATE TRIGGER trg_delete_department_cascade
AFTER DELETE ON department
FOR EACH ROW
BEGIN
    -- 删除与被删除部门关联的所有员工
    DELETE FROM employee WHERE deptId = OLD.id;
END$$

DELIMITER ;
