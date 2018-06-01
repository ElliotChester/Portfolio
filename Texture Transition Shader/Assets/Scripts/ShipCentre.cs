using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShipCentre : MonoBehaviour {

    public float rotateSpeed;

    public GameObject[] ships;

	void Start ()
    {
        EnableShip(5);
	}

	void Update ()
    {
        Rotate();
	}

    void Rotate()
    {
        transform.Rotate(0, 0, rotateSpeed * Time.deltaTime);
    }

    public void EnableShip(int shipID)
    {
        foreach (var ship in ships)
        {
            ship.GetComponent<MeshRenderer>().enabled = false;
        }

        ships[shipID].GetComponent<MeshRenderer>().enabled = true;
    }

    public void Transition()
    {
        foreach (var ship in ships)
        {
            if (this.gameObject.activeSelf == true)
            {
                ship.GetComponent<ShipShaderControl>().Transition();
            }
        }
    }

    public void SwitchTransition(int id)
    {
        foreach (var ship in ships)
        {
            ship.GetComponent<ShipShaderControl>().SwitchTransition(id);
        }
    }
}
