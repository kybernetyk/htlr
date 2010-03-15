class Hostinghash < ActiveRecord::Migration
  def self.up
	add_column "items", "hosting_hash",     :string

end

  def self.down
  end
end
