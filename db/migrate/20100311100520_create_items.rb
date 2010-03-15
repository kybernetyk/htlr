class CreateItems < ActiveRecord::Migration
  def self.up
    create_table :items do |t|
      t.column  :original_filename,  :string
      t.column  :path_to_file,       :string
      t.timestamps
    end
  end

  def self.down
    drop_table :items
  end
end
