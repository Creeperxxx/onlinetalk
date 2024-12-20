CREATE TABLE users (
    user_id BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,  -- 用户唯一标识符
    user_name VARCHAR(255) NOT NULL,                  -- 用户名
    user_passwd VARCHAR(255) NOT NULL,                -- 密码（建议使用哈希存储）
    user_email VARCHAR(255) NOT NULL,                 -- 用户电子邮件
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,  -- 创建时间
    updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,  -- 更新时间
    PRIMARY KEY (user_id),
    UNIQUE INDEX idx_user_name (user_name),           -- 确保用户名唯一
    UNIQUE INDEX idx_user_email (user_email)          -- 确保电子邮件唯一
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE messages (
    message_id BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,  -- 消息唯一标识符
    sender_id BIGINT UNSIGNED NOT NULL,                   -- 发送者ID
    receiver_id BIGINT UNSIGNED NOT NULL,                 -- 接收者ID
    sender_name VARCHAR(255) NOT NULL,                    -- 发送者名字
    receiver_name VARCHAR(255) NOT NULL,                  -- 接收者名字
    content TEXT NOT NULL,                                -- 消息内容 (可以是文本或JSON格式)
    message_type ENUM('text', 'image', 'video', 'file') NOT NULL DEFAULT 'text',  -- 消息类型
    is_delivered BOOLEAN NOT NULL DEFAULT FALSE,          -- 是否已发送(默认没发送，直到用户发消息表示也接收)
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,  -- 创建时间
    PRIMARY KEY (message_id),
    FOREIGN KEY (sender_id) REFERENCES users(user_id) ON DELETE CASCADE,  -- 外键约束
    FOREIGN KEY (receiver_id) REFERENCES users(user_id) ON DELETE CASCADE  -- 外键约束
    -- FOREIGN KEY (sender_name) REFERENCES users(user_name) ON DELETE CASCADE,  -- 外键约束
    -- FOREIGN KEY (receiver_name) REFERENCES users(user_name) ON DELETE CASCADE  -- 外键约束
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;