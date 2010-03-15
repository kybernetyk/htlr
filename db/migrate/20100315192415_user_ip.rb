class UserIp < ActiveRecord::Migration
  def self.up
	add_column "items", "uploader_ip",     :string
  end

  def self.down
  end
end
